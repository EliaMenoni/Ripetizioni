#define _GNU_SOURCE

#include "libro.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include "../unboundedqueue/util.h"

#define atoa(x) #x

static inline void LockLibrary(Libro *q) { LOCK(&q->qlock); }
static inline void UnlockLibrary(Libro *q) { UNLOCK(&q->qlock); }
static inline void UnlockLibraryAndWait(Libro *q) {
  WAIT(&q->qcond, &q->qlock);
}
static inline void UnlockLibraryAndSignal(Libro *q) {
  SIGNAL(&q->qcond);
  UNLOCK(&q->qlock);
}

void stampa_libro(Libro *libro) {
  for (int j = 0; j < libro->numero_autori; j++)
    printf("autore: %s\n", libro->autore[j]);
  printf("titolo: %s\n", libro->titolo);
  printf("editore: %s\n", libro->editore);
  printf("anno: %d\n", libro->anno);
  printf("nota: %s\n", libro->nota);
  printf("collocazione: %s\n", libro->collocazione);
  printf("luogo_pubblicazione: %s\n", libro->luogo_pubblicazione);
  printf("descrizione_fisica: %s\n", libro->descrizione_fisica);
  if (strcmp(libro->prestito, "") != 0)
    printf("prestito: %s\n", libro->prestito);
  else
    printf("prestito:\n");

  printf("\n");
}

void stampa_libreria(Nodo *libreria) {
  while (libreria != NULL) {
    stampa_libro(libreria->libro);
    libreria = libreria->next;
  }
}

char * trim(char *str) {
  char *end;
  if(str == NULL) return NULL;
  // Trim leading space
  while (*str == ' ') str++;

  if (*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && *str == ' ') end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

Libro *crea_libro_da_stringa(char *riga) {
  // Alloco memoria per libro
  Libro *in = malloc(sizeof(Libro));

  memset(in, 0, sizeof(in));
  if (in == NULL) exit(1);

  for (int i = 0; i < 5; i++) strcpy(in->autore[i], "");
  strcpy(in->collocazione, "");
  strcpy(in->descrizione_fisica, "");
  strcpy(in->editore, "");
  strcpy(in->luogo_pubblicazione, "");
  strcpy(in->nota, "");
  strcpy(in->prestito, "");
  strcpy(in->titolo, "");
  in->anno = -1;
  in->numero_autori = 0;
  // strcmp(in->prestito, "");

  int i = 0;
  char *token;
  char *save1, *save2;
  char *attributo, *dato_inserire;

  token = strtok_r(riga, ";", &save1);  // il save dice da che punto ripartire nella stringa
  while (token != NULL) {
    if (*token != '\n') {
      attributo = strtok_r(token, ":", &save2);
      attributo = trim(attributo);
      dato_inserire = strtok_r(NULL, ":", &save2);
      dato_inserire = trim(dato_inserire);
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
        strcpy(in->prestito, dato_inserire);
      }
    }
    token = strtok_r(NULL, ";", &save1);
  }
  in->numero_autori = i;
  // Restituisco il puntatore alla testa della lista
  return in;
}

Nodo *crea_catalogo_da_file(char *file) {
  Nodo *catalogo;
  Nodo *nodo, *precedente;
  // Apro il file
  FILE *in_file = fopen(file, "r");
  char riga[1000];
  catalogo = malloc(sizeof(Nodo));

  memset(catalogo, 0, sizeof(catalogo));
  if (catalogo == NULL) exit(1);

  nodo = catalogo;
  while (fgets(riga, 1000, in_file)) {
    nodo->libro = crea_libro_da_stringa(riga);
    nodo->next = malloc(sizeof(Nodo));

    memset(nodo->next, 0, sizeof(nodo->next));
    if (nodo->next == NULL) exit(1);

    precedente = nodo;
    nodo = nodo->next;
  }
  fclose(in_file);
  free(precedente->next);
  precedente->next = NULL;
  return catalogo;
}

void copia_libro(Libro *destinazione, Libro *sorgente) {
  destinazione->numero_autori = sorgente->numero_autori;
  for (int i = 0; i < sorgente->numero_autori; i++) {
    strcpy(destinazione->autore[i], sorgente->autore[i]);
  }
  strcpy(destinazione->titolo, sorgente->titolo);
  strcpy(destinazione->editore, sorgente->editore);
  destinazione->anno = sorgente->anno;
  strcpy(destinazione->nota, sorgente->nota);
  strcpy(destinazione->collocazione, sorgente->collocazione);
  strcpy(destinazione->luogo_pubblicazione, sorgente->luogo_pubblicazione);
  strcpy(destinazione->descrizione_fisica, sorgente->descrizione_fisica);
  strcpy(destinazione->prestito, sorgente->prestito);
}

Nodo *ricerca_libri(Nodo *cursore, Libro *filtri) {
  int valido;
  Nodo *invia = NULL;
  Nodo *nodo_invia = NULL;

  // Per ogni elemento della libreria
  while (cursore) {
    valido = 1;

    // Se esiste un filtro sull'autore si trova nel primo elemento dell'array
    // autore
    if (strcmp(filtri->autore[0], "") != 0) {
      int trovato = 0;
      // Cerca l'autore indicato nel nodo corrente della libreria
      for (int i = 0; i < cursore->libro->numero_autori; i++)
        if (strstr(cursore->libro->autore[i], filtri->autore[0])) trovato++;
      // Se nel libro corrente e' presente l'autore cercato
      if (trovato > 0)
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    // FILTRO TITOLO
    if (strcmp(filtri->titolo, "") != 0) {
      // guarda se esistono sottostringhe della seconda parola
      // all'interno della prima
      if (strstr(cursore->libro->titolo, filtri->titolo))
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    // FILTRO EDITORE
    if (strcmp(filtri->editore, "") !=
        0) {  // guarda se esistono sottostringhe della seconda parola
      // all'interno della prima
      if (strstr(cursore->libro->editore, filtri->editore))
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    // FILTRO ANNO
    if (filtri->anno != -1) {
      if (filtri->anno == cursore->libro->anno)
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    // FILTRO COLLOCAZIONE
    if (strcmp(filtri->collocazione, "") != 0) {
      if (strstr(cursore->libro->collocazione, filtri->collocazione))
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    // FILTRO DESCRIZIONE FISICA
    if (strcmp(filtri->descrizione_fisica, "") != 0) {
      if (strstr(cursore->libro->descrizione_fisica,
                 filtri->descrizione_fisica))
        valido = valido & 1;
      else
        valido = valido & 0;
    }
    // FILTRO PRESTITO
    if (strcmp(filtri->prestito, "") != 0) {
      if (strstr(cursore->libro->prestito, filtri->prestito))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    // Se dopo il controllo di tutti i filtri il libro e' valido
    if (valido) {
      // Guardo se la listad a inviare e' vuota
      if (invia == NULL) {
        // Se e' vuota creo il primo elemento
        invia = (Nodo *)malloc(sizeof(Nodo));

        memset(invia, 0, sizeof(invia));

        if (invia == NULL) exit(1);
        // Inizializzo il cursore
        nodo_invia = invia;
      } else {
        // Se la lista da inviare non e' vuota
        // Creo il nodo successivo
        nodo_invia->next = (Nodo *)malloc(sizeof(Nodo));

        memset(nodo_invia->next, 0, sizeof(nodo_invia->next));

        if (nodo_invia->next == NULL) exit(1);
        // Mi sposto al nodo appena creato
        nodo_invia = nodo_invia->next;
      }
      // Inserisco il libro filtrato nella lista
      nodo_invia->libro = cursore->libro;
      nodo_invia->next = NULL;
    }
    // Mi sposto al prossimo libro nella libreria
    cursore = cursore->next;
  }
  // Restituisco il puntatore alla testa della lista dei nodi che rispettano i
  // filtri
  return invia;
}

void aggiorna_scadenze_prestiti(Nodo *libreria) {
  time_t now = time(NULL);
  struct tm *today;
  today = localtime(&now);

  while (libreria != NULL) {
    LockLibrary(libreria->libro);

    if (strcmp(libreria->libro->prestito, "") != 0) {
      struct tm data;
      strptime(libreria->libro->prestito, "%S-%M-%H", &data);

      // Confronta le date
      // printf("Prestito\t\tOra\n%d\t\t%d\n%d\t\t%d\n%d\t\t%d\n", data.tm_hour,
      // today->tm_hour, data.tm_min, today->tm_min, data.tm_sec,
      // today->tm_sec);
      if (data.tm_hour < today->tm_hour ||
          (data.tm_hour == today->tm_hour && data.tm_min < today->tm_min) ||
          (data.tm_hour == today->tm_hour && data.tm_min == today->tm_min &&
           data.tm_sec < today->tm_sec)) {
        strcpy(libreria->libro->prestito, "");
      }
    }

    UnlockLibraryAndSignal(libreria->libro);
    libreria = libreria->next;
  }
}

int noleggia(Libro *libro) {
  LockLibrary(libro);
  if (strcmp(libro->prestito, "")) {
    // Si presuppone che al momento del noleggio sia gia' stat invocata la
    // aggiorna_libreria_prestiti e che quindi se libro.prestito non e' NULL e'
    // stato assegnato da un altro worker
    UnlockLibraryAndSignal(libro);
    return 0;
  } else {
    time_t now;
    struct tm *today;
    now = time(NULL);
    today = localtime(&now);

    snprintf(
        libro->prestito, 11, "%02d-%02d-%02d", (today->tm_sec + 30) % 60,
        today->tm_min + (int)((today->tm_sec + 30) / 60) % 60,
        today->tm_hour);  //+1 di base e +1 per scadenza del mese di prestito

    UnlockLibraryAndSignal(libro);
    return 1;
  }
}

char *libro_toString(Libro *libro) {
  char *output = malloc(sizeof(char) * 2048);

  memset(output, 0, 2048);
  if (output == NULL) exit(1);

  strcpy(output, "");

  for (int i = 0; i < libro->numero_autori; i++) {
    strcat(output, "autore:");
    strcat(output, libro->autore[i]);
    strcat(output, ";");
  }
  strcat(output, "titolo:");
  strcat(output, libro->titolo);
  strcat(output, ";");
  strcat(output, "collocazione:");
  strcat(output, libro->collocazione);
  strcat(output, ";");
  strcat(output, "descrizione_fisica:");
  strcat(output, libro->descrizione_fisica);
  strcat(output, ";");
  strcat(output, "editore:");
  strcat(output, libro->editore);
  strcat(output, ";");
  strcat(output, "luogo_pubblicazione:");
  strcat(output, libro->luogo_pubblicazione);
  strcat(output, ";");
  strcat(output, "nota:");
  strcat(output, libro->nota);
  strcat(output, ";");
  if (strcmp(libro->prestito, "") != 0) {
    strcat(output, "prestito:");
    strcat(output, libro->prestito);
    strcat(output, ";");
  }
  char anno[5];
  sprintf(anno, "%04d", libro->anno);
  strcat(output, "anno:");
  strcat(output, anno);
  strcat(output, ";");

  return output;
}

void delete_libreria(Nodo *libreria) {
  Nodo *prev;
  while (libreria) {
    prev = libreria;
    libreria = libreria->next;
    free(prev->libro);
    prev->libro = NULL;
    free(prev);
    prev = NULL;
  }
}