#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../lib/libro/libro.h"
#include "../lib/unboundedqueue/unboundedqueue.h"

struct Request
{
  Nodo *radice_libreria;
  int noleggio;
  Libro *filtro;
};

typedef struct Request Request;

pthread_mutex_t buffer_mutex;

volatile sig_atomic_t terminazione = 0;

void termina_processo(int signum)
{
  printf("Termino il server\n");
  terminazione = 1;
}

void *worker(void *arg)
{
  pid_t tid = syscall(SYS_gettid); // prende l'id del sottoprocesso corrente
  char *out_buffer = malloc(250);  // il buffer in cui mettiamo la stringa formattata da stampare nel log
  Queue_t *coda = (Queue_t *)arg;

  while (!terminazione)
  {
    Request *richiesta = (Request *)pop(coda);
    Nodo *libreria = richiesta->radice_libreria;
    Nodo *risultato = ricerca_libri(libreria, richiesta->filtro);

    if (richiesta->noleggio == 1)
    {
      sprintf(out_buffer, "%d - servo richiesta noleggio", tid); // fa il lavoro della printf ma anzichè stamparla la mette in un buffer
      write_log(out_buffer);
      Nodo *cursore = risultato;
      Nodo *noleggiati = NULL;
      Nodo *noleggiati_cursore = NULL;
      aggiorna_scadenza(libreria);
      while (cursore != NULL)
      {
        if (noleggia(cursore->libro))
        {
          if (noleggiati == NULL)
          {
            noleggiati = malloc(sizeof(Nodo));
            noleggiati_cursore = noleggiati;
            noleggiati->libro = cursore->libro;
            noleggiati->next = NULL;
          }
          else
          {
            noleggiati_cursore->next = malloc(sizeof(Nodo));
            noleggiati_cursore = noleggiati_cursore->next;
            noleggiati_cursore->libro = cursore->libro;
            noleggiati_cursore->next = NULL;
          }
        }
        cursore = cursore->next;
      }
      generate_log(noleggiati);
      // Invio risposta
    }
    generate_log(risultato);
    // Invio risposta
  }
}

int main(void)
{
  Nodo *testa_lista; // puntatore alla radice della lista

  testa_lista = riempi_catalogo("../data/biblioteca_1.txt");

  if (pthread_mutex_init(&buffer_mutex, NULL) != 0)
  {
    perror("pthread_mutex_init() error");
    exit(1);
  }

  int num_workers = 5;
  if (num_workers <= 0)
    return 0;
  pthread_t *workers = malloc(sizeof(pthread_t) * num_workers);

  // crea il buffer
  Queue_t *buffer = initQueue();

  FILE *log = fopen("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibserver/logs/requests.log", "w");
  fclose(log);

  Request *richiesta = (Request *)malloc(sizeof(Request));
  Libro filtro;
  strcpy(filtro.autore[0], "");
  strcpy(filtro.titolo, "arte della");
  strcpy(filtro.editore, "");
  filtro.anno = -1;
  strcpy(filtro.collocazione, "");
  strcpy(filtro.descrizione_fisica, "");
  filtro.prestito = NULL;
  richiesta->filtro = &filtro;
  richiesta->noleggio = 1;
  richiesta->radice_libreria = testa_lista;
  push(buffer, (void *)richiesta);
  push(buffer, (void *)richiesta);
  push(buffer, (void *)richiesta);
  push(buffer, (void *)richiesta);
  push(buffer, (void *)richiesta);
  write_log("Server avviato");

  // avvia m worker
  for (int i = 0; i < num_workers; i++)
  {
    if (pthread_create(&(workers[i]), NULL, worker, buffer) != 0)
    {
      perror("pthread_create() error");
      exit(1);
    }
  }

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = termina_processo;
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGINTR, &action, NULL);

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
  deleteQueue(buffer);

  return 0;
}