#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../lib/libro/libro.h"
#include "../lib/unboundedqueue/unboundedqueue.h"

struct Request
{
  Nodo *radice_libreria;
  int noleggio;
  Libro *filtro;
  int connection_number;
};

typedef struct Request Request;

pthread_mutex_t buffer_mutex;

volatile int terminazione = 0;

void termina_processo(int signum)
{
  write_log("Terminazione Server", 0);
  terminazione = 1;
}

void *worker(void *arg)
{
  pid_t tid = syscall(SYS_gettid); // prende l'id del sottoprocesso corrente
  char *out_buffer = malloc(250);  // il buffer in cui mettiamo la stringa formattata da stampare nel log
  if (out_buffer == NULL)
    exit(1);

  Queue_t *coda = (Queue_t *)arg;

  printf("Worker %d avviato\n", tid);
  sprintf(out_buffer, "Worker %d avviato", tid);
  write_log(out_buffer, 0);

  while (!terminazione || length(coda) > 0)
  {
    if (length(coda) == 0)
    {
      //printf("Worker %d e' in attesa di richieste\n", tid);
      continue;
    }
    printf("Worker %d serve una richiesta\n", tid);
    Request *richiesta = (Request *)pop(coda);
    Nodo *libreria = richiesta->radice_libreria;

    Nodo *risultato = ricerca_libri(libreria, richiesta->filtro);

    if (richiesta->noleggio == 1)
    {
      sprintf(out_buffer, "%d - servo richiesta noleggio", tid); // fa il lavoro della printf ma anzichè stamparla la mette in un buffer
      write_log(out_buffer, 0);
      Nodo *cursore = risultato;
      Nodo *noleggiati = NULL;
      Nodo *noleggiati_cursore = NULL;
      aggiorna_scadenze_prestiti(libreria);
      while (cursore != NULL)
      {
        if (noleggia(cursore->libro))
        {
          if (noleggiati == NULL)
          {
            noleggiati = malloc(sizeof(Nodo));
            if (noleggiati == NULL)
              exit(1);
            noleggiati_cursore = noleggiati;
            noleggiati->libro = cursore->libro;
            noleggiati->next = NULL;
          }
          else
          {
            noleggiati_cursore->next = malloc(sizeof(Nodo));
            if (noleggiati_cursore->next == NULL)
              exit(1);
            noleggiati_cursore = noleggiati_cursore->next;
            noleggiati_cursore->libro = cursore->libro;
            noleggiati_cursore->next = NULL;
          }
        }
        cursore = cursore->next;
      }
      write_log(noleggiati, 2);

      cursore = noleggiati;
      while (cursore)
      {
        // stampa_libro(cursore->libro);
        send(richiesta->connection_number, cursore->libro, sizeof(Libro), 0);
        cursore = cursore->next;
      }
    }
    else
    {
      write_log(risultato, 1);
      while (risultato)
      {
        stampa_libro(risultato->libro);
        send(richiesta->connection_number, risultato->libro, sizeof(Libro), 0);
        risultato = risultato->next;
      }
    }
    // Invio risposta

    // if (richiesta->filtro->prestito != NULL)
    //   free(richiesta->filtro->prestito);
    // free(richiesta);
    // richiesta = NULL;
    Libro stream_end;
    stream_end.numero_autori = -1;
    send(richiesta->connection_number, &stream_end, sizeof(Libro), 0);
    close(richiesta->connection_number);
    free(richiesta);
  }
}

int main(void)
{
  Nodo *testa_lista; // puntatore alla radice della lista

  printf("CARICAMENTO CATALOGO DA FILE\n");
  testa_lista = crea_catalogo_da_file("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibserver/data/biblioteca_1.txt");

  printf("INIZIALIZZO PTHREAD MUTEX\n");
  if (pthread_mutex_init(&buffer_mutex, NULL) != 0)
  {
    perror("pthread_mutex_init() error");
    exit(1);
  }

  int num_workers = 5;
  if (num_workers <= 0)
    return 0;
  printf("CREATO ARRAY GESTIONE THREAD\n");
  pthread_t *workers = malloc(sizeof(pthread_t) * num_workers);
  if (workers == NULL)
    exit(1);

  // crea il buffer
  printf("CREO QUEUE PER GESTIONE RICHIESTE\n");
  Queue_t *buffer = initQueue();

  printf("PULIZIA LOG\n");
  FILE *log = fopen("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibserver/logs/requests.log", "w");
  fclose(log);

  // push(buffer, (void *)richiesta);
  // push(buffer, (void *)richiesta);

  write_log("Avvio Server", 0);

  printf("AVVIO I WORKER\n");
  write_log("Avvio i Worker", 0);
  // avvia m worker
  for (int i = 0; i < num_workers; i++)
  {
    if (pthread_create(&(workers[i]), NULL, worker, buffer) != 0)
    {
      perror("pthread_create() error");
      exit(1);
    }
  }

  printf("REGISTRAZIONE ASCOLTO SEGNALI DI SISTEMA\n");
  write_log("Registrazione segnali di terminazione", 0);
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = termina_processo;
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGINT, &action, NULL);

  // INIZIO ASCOLTO SOCKET
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(8002);

  // Forcefully attaching socket to the port 8000
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  struct Request *connection_request;
  while (!terminazione)
  {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    connection_request = malloc(sizeof(Request));
    if (connection_request == NULL)
      exit(1);
    Libro filtro;
    int noleggio;

    read(new_socket, &filtro, sizeof(Libro)); // tramite la read inserisco i dati ricevuti da new_socket dentro connection request, della dimensione di Request
    read(new_socket, &noleggio, sizeof(int));

    Libro *request_filter = malloc(sizeof(Libro));
    if(request_filter == NULL) exit(1);
    copia_libro(request_filter, &filtro);

    connection_request->filtro = request_filter;
    connection_request->noleggio = noleggio;
    connection_request->connection_number = new_socket;
    connection_request->radice_libreria = testa_lista;
    
    push(buffer, (void *)connection_request);
    // Leggo i dati ricevuto dal client
    // valread = read(new_socket, buffer, 1024);
    // printf("%s\n", buffer);
    // Invio la risposta al client
    // send(new_socket, hello, strlen(hello), 0);
    // closing the connected socket
    // close(new_socket);
  }
  // closing the listening socket
  shutdown(server_fd, SHUT_RDWR);

  printf("ASPETTO I WORKER PER LA TERMINAZIONE\n");
  // aspetto i worker
  for (int i = 0; i < num_workers; i++)
  {
    pthread_mutex_lock(&buffer_mutex);
    if (pthread_join(workers[i], NULL) != 0)
    {
      perror("pthread_create() error");
      exit(1);
    }
    pthread_mutex_unlock(&buffer_mutex);
  }
  free(workers);
  pthread_mutex_destroy(&buffer_mutex);

  // Salvare la Libreria

  deleteQueue(buffer);

  return 0;
}