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

struct Nodo {
  Libro* libro;
  struct Nodo* next;
};

typedef struct Nodo Nodo;

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
  //stampa(in);
  return in;
}

Nodo* riempi_catalogo(char *file) {
  Nodo *catalogo;
  Nodo *nodo, *precedente;
  FILE *in_file = fopen(file, "r");
  char riga[1000];
  catalogo = malloc(sizeof(Nodo));
  nodo = catalogo;
  while (fgets(riga, 1000, in_file)) {
    nodo -> libro = crea_libro(riga);
    nodo -> next = malloc(sizeof(Nodo));
    precedente = nodo;
    nodo = nodo -> next;
  }
  fclose(in_file);
  precedente -> next = NULL;
  return catalogo;
}



Nodo* ricerca_libri(Nodo* testa_lista, Libro* filtri){
   int valido;
   Nodo* invia = NULL;
   Nodo* nodo_invia = NULL;
   
   while (testa_lista){
    valido = 1;
    
    if(strcmp(filtri -> autore[0], "") != 0){
      int trovato = 0;
      for(int i = 0; i < testa_lista -> libro -> numero_autori; i++)
        if(strstr(testa_lista -> libro -> autore[i], filtri -> autore[0]))
          trovato++;
      if(trovato > 0)
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    
    if(strcmp(filtri -> titolo, "") != 0){  //guarda se esistono sottostringhe della seconda parola all'interno della prima
      if(strstr(testa_lista -> libro -> titolo, filtri -> titolo))
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    
    if(strcmp(filtri -> editore, "") != 0){  //guarda se esistono sottostringhe della seconda parola all'interno della prima
      if(strstr(testa_lista -> libro -> editore, filtri -> editore))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if(filtri -> anno != -1){
      if(filtri -> anno == testa_lista -> libro -> anno)
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if(strcmp(filtri -> collocazione, "") != 0){  
      if(strstr(testa_lista -> libro -> collocazione, filtri -> collocazione))
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    
    if(strcmp(filtri -> descrizione_fisica, "") != 0){  
      if(strstr(testa_lista -> libro -> descrizione_fisica, filtri -> descrizione_fisica))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if(filtri -> prestito != NULL){
      if(strstr(testa_lista -> libro -> prestito, filtri -> prestito))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if(valido){
      if(invia == NULL){
        invia = (Nodo *)malloc(sizeof(Nodo));
        nodo_invia = invia;
      }
      else {
        nodo_invia -> next = (Nodo *)malloc(sizeof(Nodo));
        nodo_invia = nodo_invia -> next;
      }
      
      Libro* tmp = (Libro *)malloc(sizeof(Libro));
      copia(&tmp, testa_lista);
      nodo_invia -> libro = tmp;
      nodo_invia -> next = NULL;
    } 
    testa_lista = testa_lista -> next;
   }
   return invia;
}

int main(void) {
  Nodo* testa_lista; //puntatore alla radice della lista
  
  testa_lista = riempi_catalogo("../data/biblioteca_1.txt");
//  Nodo* nodo = testa_lista;
//  while(nodo != NULL){
//    stampa(nodo->libro);
//    nodo = nodo -> next;
//  }
  Libro filtro;
  strcpy(filtro.autore[0], "");
  strcpy(filtro.titolo, "");
  strcpy(filtro.editore, "");
  filtro.anno = -1;
  strcpy(filtro.collocazione, "");
  strcpy(filtro.descrizione_fisica, "");
  filtro.prestito = NULL;

  Nodo* ricerca = ricerca_libri(testa_lista, &filtro);

  Nodo* nodo = ricerca;
  while(nodo != NULL){
    stampa(nodo->libro);
    nodo = nodo -> next;
  }
  
  return 0;
}