#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "../lib/comprot/comprot.h"
#include "../lib/libro/libro.h"
#include "../lib/unboundedqueue/unboundedqueue.h"
#include "../lib/factory/factory.h"

#define PORT 8003

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Format: server bib_name record_file W\n");
    return 1;
  }

  write_log(argv[1], 0);

  Nodo *testa_lista; // puntatore alla radice della lista

  printf("CARICAMENTO CATALOGO DA FILE\n");
  testa_lista = crea_catalogo_da_file(argv[2]);

  printf("INIZIALIZZO PTHREAD MUTEX\n");
  if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
    perror("pthread_mutex_init() error");
    exit(1);
  }

  int num_workers = atoi(argv[3]);
  if (num_workers <= 0) {
    printf("W must be > 0\n");
    return 0;
  }
  printf("CREATO ARRAY GESTIONE THREAD\n");
  pthread_t *workers = malloc(sizeof(pthread_t) * num_workers);
  
  memset(workers, 0, sizeof(workers));
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
  for (int i = 0; i < num_workers; i++) {
    if (pthread_create(&(workers[i]), NULL, worker, buffer) != 0) {
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

  server_fd = create_socket_file();
  setup_socket_data("127.0.0.1", PORT, &address);
  socket_bind(server_fd, &address);
  server_listen(server_fd);

  struct DataPool *connection_request;
  while (!terminazione) {
    new_socket = server_accept(server_fd, &address);

    connection_request = server_read(new_socket, testa_lista);

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
  for (int i = 0; i < num_workers; i++) {
    pthread_mutex_lock(&buffer_mutex);
    if (pthread_join(workers[i], NULL) != 0) {
      perror("pthread_create() error");
      exit(1);
    }
    pthread_mutex_unlock(&buffer_mutex);
  }
  free(workers);
  workers = NULL;
  pthread_mutex_destroy(&buffer_mutex);

  // Salvare la Libreria

  deleteQueue(buffer);
  delete_libreria(testa_lista);
  return 0;
}