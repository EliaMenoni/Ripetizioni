#ifndef FACTORY_H_
#define FACTORY_H_

#include "../libro/libro.h"

struct DataPool {              //struct che usiamo per comunicare con i thread. Il server, dopo aver ricevuto una struct Packet, popola una
  Nodo *radice_libreria;       //DataPool e la inserisce nella coda
  int noleggio;
  Libro *filtro;
  int connection_number;       //abbiamo la libreria, un intero che indica se è una richiesta di noleggio, i filtri con cui effettuiamo la ricerca e il numero del socket
};
typedef struct DataPool DataPool;

char file_name[250];

pthread_mutex_t buffer_mutex;   
volatile int terminazione;     // dice ai thread di fermarsi quando riceve CTRL+c

void termina_processo(int signum);   //modifica volatile int terminazione quando si riceve CTRL+c

void write_log(void* data, int type);

DataPool *server_read(int socket, Nodo *libreria);  //legge dal socket la Packet e crea la DataPool

void *worker(void *arg);  //thread vero e proprio

#endif