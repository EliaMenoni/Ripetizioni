#ifndef FACTORY_H_
#define FACTORY_H_

#include "../libro/libro.h"

struct DataPool {
  Nodo *radice_libreria;
  int noleggio;
  Libro *filtro;
  int connection_number;
};
typedef struct DataPool DataPool;

pthread_mutex_t buffer_mutex;
volatile int terminazione;

void termina_processo(int signum);

void write_log(void* data, int type);

DataPool *server_read(int socket, Nodo *libreria);

void *worker(void *arg);

#endif