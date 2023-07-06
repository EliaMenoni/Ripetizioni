#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void stampa(Libro *libro) {
  for (int j = 0; j < libro->numero_autori; j++)
    printf("autore: %s\n", libro->autore[j]);
  printf("titolo: %s\n", libro->titolo);
  printf("editore: %s\n", libro->editore);
  printf("anno: %d\n", libro->anno);
  printf("nota: %s\n", libro->nota);
  printf("collocazione: %s\n", libro->collocazione);
  printf("luogo_pubblicazione: %s\n", libro->luogo_pubblicazione);
  printf("descrizione_fisica: %s\n", libro->descrizione_fisica);
  if (libro->prestito != NULL)
    printf("prestito: %s\n", libro->prestito);
  else
    printf("prestito:\n");

  printf("\n");
}

Libro *crea_libro(char *riga) {
  Libro *in = (Libro *)malloc(sizeof(Libro));
  in->prestito = NULL;
  int i = 0;
  char *token;
  char *save1, *save2;
  char *attributo, *dato_inserire;
  token = strtok_r(
      riga, ";", &save1);  // il save dice da che punto ripartire nella stringa
  while (token != NULL) {
    if (*token != '\n') {
      attributo = strtok_r(token, ":", &save2);
      dato_inserire = strtok_r(NULL, ":", &save2);

      if (strcmp(attributo, "autore") == 0) {
        strcpy(in->autore[i], dato_inserire);
        i++;
      } else if (strcmp(attributo, "titolo") == 0)
        strcpy(in->titolo, dato_inserire);
      else if (strcmp(attributo, "editore") == 0)
        strcpy(in->editore, dato_inserire);
      else if (strcmp(attributo, "anno") == 0)
        in->anno = atoi(dato_inserire);
      else if (strcmp(attributo, "nota") == 0)
        strcpy(in->nota, dato_inserire);
      else if (strcmp(attributo, "collocazione") == 0)
        strcpy(in->collocazione, dato_inserire);
      else if (strcmp(attributo, "luogo_pubblicazione") == 0)
        strcpy(in->luogo_pubblicazione, dato_inserire);
      else if (strcmp(attributo, "descrizione_fisica") == 0)
        strcpy(in->descrizione_fisica, dato_inserire);
      else if (strcmp(attributo, "prestito") == 0) {
        in->prestito = (char *)malloc(sizeof(char) * 11);
        strcpy(in->prestito, dato_inserire);
      }
    }
    token = strtok_r(NULL, ";", &save1);
  }
  in->numero_autori = i;
  stampa(in);
  return NULL;
}

int riempi_catalogo(char *file, Libro *catalogo) {
  FILE *in_file = fopen(file, "r");
  char riga[1000];
  while (fgets(riga, 1000, in_file)) {
    crea_libro(riga);
  }
  fclose(in_file);
}

int main(void) {
  riempi_catalogo("../data/biblioteca_1.txt", NULL);
  return 0;
}