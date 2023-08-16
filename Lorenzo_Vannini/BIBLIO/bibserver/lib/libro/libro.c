#include "libro.h"
#include "../unboundedqueue/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

static inline void LockLibrary(Libro *q)            { LOCK(&q->qlock);   }
static inline void UnlockLibrary(Libro *q)          { UNLOCK(&q->qlock); }
static inline void UnlockLibraryAndWait(Libro *q)   { WAIT(&q->qcond, &q->qlock); }
static inline void UnlockLibraryAndSignal(Libro *q) { SIGNAL(&q->qcond); UNLOCK(&q->qlock); }

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

void stampa_libreria(Nodo *libreria) {
  while(libreria != NULL){
  for (int j = 0; j < libreria->libro->numero_autori; j++)
    printf("autore: %s\n", libreria->libro->autore[j]);
  printf("titolo: %s\n", libreria->libro->titolo);
  printf("editore: %s\n", libreria->libro->editore);
  printf("anno: %d\n", libreria->libro->anno);
  printf("nota: %s\n", libreria->libro->nota);
  printf("collocazione: %s\n", libreria->libro->collocazione);
  printf("luogo_pubblicazione: %s\n", libreria->libro->luogo_pubblicazione);
  printf("descrizione_fisica: %s\n", libreria->libro->descrizione_fisica);
  if (libreria->libro->prestito != NULL)
    printf("prestito: %s\n", libreria->libro->prestito);
  else
    printf("prestito:\n");

  printf("\n");
  libreria = libreria->next;
  }
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
  // stampa(in);
  return in;
}

Nodo *riempi_catalogo(char *file) {
  Nodo *catalogo;
  Nodo *nodo, *precedente;
  FILE *in_file = fopen(file, "r");
  char riga[1000];
  catalogo = malloc(sizeof(Nodo));
  nodo = catalogo;
  while (fgets(riga, 1000, in_file)) {
    nodo->libro = crea_libro(riga);
    nodo->next = malloc(sizeof(Nodo));
    precedente = nodo;
    nodo = nodo->next;
  }
  fclose(in_file);
  precedente->next = NULL;
  return catalogo;
}

void copia(Libro *destinazione, Libro *sorgente) {
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
  if (sorgente->prestito != NULL)
    strcpy(destinazione->prestito, sorgente->prestito);
  else
    destinazione->prestito = NULL;
}

Nodo *ricerca_libri(Nodo *cursore, Libro *filtri) {
  int valido;
  Nodo *invia = NULL;
  Nodo *nodo_invia = NULL;

  while (cursore) {
    valido = 1;

    if (strcmp(filtri->autore[0], "") != 0) {
      int trovato = 0;
      for (int i = 0; i < cursore->libro->numero_autori; i++)
        if (strstr(cursore->libro->autore[i], filtri->autore[0])) trovato++;
      if (trovato > 0)
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (strcmp(filtri->titolo, "") != 0) {  // guarda se esistono sottostringhe della seconda parola
                                            // all'interno della prima
      if (strstr(cursore->libro->titolo, filtri->titolo))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (strcmp(filtri->editore, "") != 0) {  // guarda se esistono sottostringhe della seconda parola
              // all'interno della prima
      if (strstr(cursore->libro->editore, filtri->editore))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (filtri->anno != -1) {
      if (filtri->anno == cursore->libro->anno)
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (strcmp(filtri->collocazione, "") != 0) {
      if (strstr(cursore->libro->collocazione, filtri->collocazione))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (strcmp(filtri->descrizione_fisica, "") != 0) {
      if (strstr(cursore->libro->descrizione_fisica,
                 filtri->descrizione_fisica))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (filtri->prestito != NULL) {
      if (strstr(cursore->libro->prestito, filtri->prestito))
        valido = valido & 1;
      else
        valido = valido & 0;
    }

    if (valido) {
      if (invia == NULL) {
        invia = (Nodo *)malloc(sizeof(Nodo));
        nodo_invia = invia;
      } else {
        nodo_invia->next = (Nodo *)malloc(sizeof(Nodo));
        nodo_invia = nodo_invia->next;
      }

      //Libro *tmp = (Libro *)malloc(sizeof(Libro));
      //copia(tmp, cursore->libro);
      nodo_invia->libro = cursore -> libro;
      nodo_invia->next = NULL;
    }
    cursore = cursore->next;
  }
  return invia;
}

void aggiorna_scadenza(Nodo *libreria){
  time_t now = time(NULL);
  struct tm* today;
  today = localtime(&now);

  while(libreria != NULL){
    LockLibrary(libreria->libro);
    if(libreria->libro->prestito != NULL){
      struct tm data;
      strptime(libreria->libro->prestito, "%d/%m/%Y", &data);

      // Confronta le date
      if(data.tm_year < today->tm_year ||
        (data.tm_year == today->tm_year && data.tm_mon < today->tm_mon) ||
        (data.tm_year == today->tm_year && data.tm_mon == today->tm_mon && data.tm_mday < today->tm_mday)) {
        free(libreria->libro->prestito); // La data fornita è precedente a quella attuale
        libreria->libro->prestito = NULL;
      }
    }
    UnlockLibraryAndSignal(libreria->libro);
    libreria = libreria->next;
  }
}

int noleggia(Libro* libro){
  LockLibrary(libro);
  if(libro -> prestito != NULL) {
    UnlockLibraryAndSignal(libro);
    return 0;
  }
  else {
    time_t now;
    struct tm* today;
    now = time(NULL);
    today = localtime(&now);
    libro -> prestito = malloc(sizeof(char)*11);
    snprintf(libro->prestito, 11, "%02d/%02d/%04d", today->tm_mday, today->tm_mon + 2, today->tm_year + 1900); //+1 di base e +1 per scadenza del mese di prestito
    UnlockLibraryAndSignal(libro);
    return 1;
    }
  }

  void write_log(char* text){               //apre un file e scrive una stringa data da noi, dobbiamo sfruttare lock e unlock perchè non possiamo accedere a un file con più thread contemporaneamente
    static pthread_mutex_t qlock;           //per rendere il semaforo disponibile a tutti i thread, anzichè passarlo come argomento, lo abbiamo reso statico
    static pthread_cond_t  qcond;           //viene creato all'avvio dell'esecuzione del programma e rimane disponibile dentro la funzione write_log
    LOCK(&qlock);

    FILE* log;
    log = fopen("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibserver/logs/requests.log", "a+"); //apro il file con "a+" perchè permette di aprire il file in scrittura senza cancellare il contenuto e posizionandosi alla fine.
    if(log == NULL)                                                                                  //mentre con "w" il contenuto veniva cancellato ogni volta, stampando solo l'ultima esecuzione
      exit(1);
    fprintf(log, text);                     //scrivo nel file di log il testo dato da noi e mando a capo, poi chiudo il file
    fprintf(log, "\n");
    fclose(log);
    UNLOCK(&qlock);                         //sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
    SIGNAL(&qcond);
  }

  void generate_log(Nodo* risultato){
    if(risultato == NULL)
      return;

    int n_libri = 0;
    Nodo* iteratore = risultato;
    while(iteratore != NULL){
      n_libri++;
      iteratore = iteratore->next;
    }

    char buffer[100];
    sprintf(buffer, "%d - Numero di libri presenti: %d", syscall(SYS_gettid), n_libri);
    write_log(buffer);
}