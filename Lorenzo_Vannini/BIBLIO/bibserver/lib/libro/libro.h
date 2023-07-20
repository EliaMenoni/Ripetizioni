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
  char *prestito;
};

typedef struct Libro Libro;

struct Nodo {
  Libro *libro;
  struct Nodo *next;
};

typedef struct Nodo Nodo;

void stampa(Libro *libro);

Libro *crea_libro(char *riga);

Nodo *riempi_catalogo(char *file);

void copia(Libro *destinazione, Libro *sorgente);

Nodo *ricerca_libri(Nodo *cursore, Libro *filtri);

#endif