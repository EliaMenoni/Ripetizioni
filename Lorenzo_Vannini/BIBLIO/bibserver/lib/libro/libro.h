#include <pthread.h>
#ifndef LIBRO_H_
#define LIBRO_H_

struct Libro {
  int numero_autori;
  char autore[5][50];
  char titolo[100];
  char editore[50];
  int anno;
  char nota[250];
  char collocazione[10];
  char luogo_pubblicazione[20];
  char descrizione_fisica[250];
  char prestito[12];
  pthread_mutex_t qlock;
  pthread_cond_t  qcond;
};

typedef struct Libro Libro;

struct Nodo {
  Libro *libro;
  struct Nodo *next;
};

typedef struct Nodo Nodo;

static inline void LockLibrary(Libro *q);
static inline void UnlockLibrary(Libro *q);
static inline void UnlockLibraryAndWait(Libro *q);
static inline void UnlockLibraryAndSignal(Libro *q);

void stampa_libro(Libro *libro);

void stampa_libreria(Nodo *libreria);

Libro *crea_libro_da_stringa(char *riga);

Nodo *crea_catalogo_da_file(char *file);

void copia_libro(Libro *destinazione, Libro *sorgente);

Nodo *ricerca_libri(Nodo *libreria, Libro *filtri);

int noleggia(Libro* libro);

void aggiorna_scadenze_prestiti(Nodo* libreria);

char *libro_toString(Libro *libro);

void delete_libreria(Nodo *libreria);

char * trim(char *str);

#endif