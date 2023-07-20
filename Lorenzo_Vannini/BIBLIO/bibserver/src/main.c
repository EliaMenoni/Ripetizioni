#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../lib/libro/libro.h"
#include "../lib/unboundedqueue/unboundedqueue.h"

struct Request {
  Nodo* radice_libreria;
  int noleggio;
  Libro* filtro;
};

typedef struct Request Request;

pthread_mutex_t buffer_mutex;

void* worker(void* arg){
  printf("testing\n");
  Queue_t* coda = (Queue_t*)arg;

  Request* richiesta = (Request*)pop(coda);
  Nodo* libreria = richiesta -> radice_libreria;
}

int main(void) {
  Nodo* testa_lista; //puntatore alla radice della lista
  
  testa_lista = riempi_catalogo("../data/biblioteca_1.txt");

  if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
    perror("pthread_mutex_init() error");
    exit(1);
  }

  int num_workers = 5;
  if (num_workers <= 0) return 0;
  pthread_t *workers = malloc(sizeof(pthread_t) * num_workers);

  // crea il buffer
  Queue_t *buffer = initQueue();

  // avvia m worker
  for (int i = 0; i < num_workers; i++) {
    if (pthread_create(&(workers[i]), NULL, worker, buffer) != 0) {
      perror("pthread_create() error");
      exit(1);
    }
  }

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
  pthread_mutex_destroy(&buffer_mutex);
  deleteQueue(buffer);

  return 0;
}