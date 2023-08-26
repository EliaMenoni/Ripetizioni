#define _GNU_SOURCE

#include "libro.h"
#include "../unboundedqueue/util.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

static inline void LockLibrary(Libro *q) { LOCK(&q->qlock); }
static inline void UnlockLibrary(Libro *q) { UNLOCK(&q->qlock); }
static inline void UnlockLibraryAndWait(Libro *q) { WAIT(&q->qcond, &q->qlock); }
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

Libro *crea_libro_da_stringa(char *riga) {
  // Alloco memoria per libro
  Libro *in = (Libro *)malloc(sizeof(Libro));
  if (in == NULL)
    exit(1);

  strcmp(in->prestito, "");

  int i = 0;
  char *token;
  char *save1, *save2;
  char *attributo, *dato_inserire;

  token = strtok_r(riga, ";", &save1); // il save dice da che punto ripartire nella stringa
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
  if (catalogo == NULL)
    exit(1);

  nodo = catalogo;
  while (fgets(riga, 1000, in_file)) {
    nodo->libro = crea_libro_da_stringa(riga);
    nodo->next = malloc(sizeof(Nodo));
    if (nodo->next == NULL)
      exit(1);

    precedente = nodo;
    nodo = nodo->next;
  }
  fclose(in_file);
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

    // Se esiste un filtro sull'autore si trova nel primo elemento dell'array autore
    if (strcmp(filtri->autore[0], "") != 0) {
      int trovato = 0;
      // Cerca l'autore indicato nel nodo corrente della libreria
      for (int i = 0; i < cursore->libro->numero_autori; i++)
        if (strstr(cursore->libro->autore[i], filtri->autore[0]))
          trovato++;
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
        0) { // guarda se esistono sottostringhe della seconda parola
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
        if (invia == NULL)
          exit(1);
        // Inizializzo il cursorehttps://prod.liveshare.vsengsaas.visualstudio.com/join?8D576F99B8E14100306451BAEEC171D975C1
        nodo_invia = invia;
      } else {
        // Se la lista da inviare non e' vuota
        // Creo il nodo successivo
        nodo_invia->next = (Nodo *)malloc(sizeof(Nodo));
        if (nodo_invia->next == NULL)
          exit(1);
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
  // Restituisco il puntatore alla testa della lista dei nodi che rispettano i filtri
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
      strptime(libreria->libro->prestito, "%d/%m/%Y", &data);

      // Confronta le date
      if (data.tm_year < today->tm_year ||
          (data.tm_year == today->tm_year && data.tm_mon < today->tm_mon) ||
          (data.tm_year == today->tm_year && data.tm_mon == today->tm_mon && data.tm_mday < today->tm_mday)) {
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
    // Si presuppone che al momento del noleggio sia gia' stat invocata la aggiorna_libreria_prestiti e che quindi se libro.prestito non e' NULL e' stato assegnato da un altro worker
    UnlockLibraryAndSignal(libro);
    return 0;

  } else {
    time_t now;
    struct tm *today;
    now = time(NULL);
    today = localtime(&now);

    snprintf(libro->prestito, 11, "%02d/%02d/%04d", today->tm_mday, today->tm_mon + 2, today->tm_year + 1900); //+1 di base e +1 per scadenza del mese di prestito

    UnlockLibraryAndSignal(libro);
    return 1;
  }
}

void write_log(void *data, int type) { // apre un file e scrive una stringa data da noi, dobbiamo sfruttare lock e unlock perchè non possiamo accedere a un file con più thread contemporaneamente
  static pthread_mutex_t qlock;        // per rendere il semaforo disponibile a tutti i thread, anzichè passarlo come argomento, lo abbiamo reso statico
  static pthread_cond_t qcond;         // viene creato all'avvio dell'esecuzione del programma e rimane disponibile dentro la funzione write_log
  LOCK(&qlock);
  FILE *log;
  log = fopen("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibserver/logs/requests.log", "a+"); // apro il file con "a+" perchè permette di aprire il file in scrittura senza cancellare il contenuto e posizionandosi alla fine.
  if (log == NULL)                                                                                 // mentre con "w" il contenuto veniva cancellato ogni volta, stampando solo l'ultima esecuzione
    exit(1);

  if (type == 0) { // Stringa
    char *text = (char *)data;

    fprintf(log, "%s\n", text); // scrivo nel file di log il testo dato da noi e mando a capo, poi chiudo il file

  } else if (type == 1) { // Risultato Operazione Query
    Nodo *libri = (Nodo *)data;

    if (libri == NULL) {
      fprintf(log, "%ld - Numero di libri inviati: 0\n", syscall(SYS_gettid));
      fclose(log);
      UNLOCK(&qlock); // sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
      SIGNAL(&qcond);
      return;
    }

    int n_libri = 0;
    Nodo *iteratore = libri;
    while (iteratore != NULL) {
      n_libri++;
      iteratore = iteratore->next;
      // da aggiungere i dati
    }

    fprintf(log, "%ld - Numero di libri inviati: %d\n", syscall(SYS_gettid), n_libri);

    iteratore = libri;
    while (iteratore) {
      fprintf(log, "\t- %s di (1* autore) %s anno %d noleggio %s", iteratore->libro->titolo, iteratore->libro->autore[0], iteratore->libro->anno, iteratore->libro->prestito);
      if (iteratore->next != NULL)
        fprintf(log, "\n");
      iteratore = iteratore->next;
    }
  } else if (type == 2) { // Risultato Operazione Noleggio
    Nodo *libri = (Nodo *)data;

    if (libri == NULL) {
      fprintf(log, "%ld - Numero di libri noleggiati: 0\n", syscall(SYS_gettid));
      fclose(log);
      UNLOCK(&qlock); // sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
      SIGNAL(&qcond);
      return;
    }

    int n_libri = 0;
    Nodo *iteratore = libri;
    while (iteratore != NULL) {
      n_libri++;
      iteratore = iteratore->next;
      // da aggiungere i dati
    }

    fprintf(log, "%ld - Numero di libri noleggiati: %d\n", syscall(SYS_gettid), n_libri);

    iteratore = libri;
    while (iteratore) {
      fprintf(log, "\t- %s di (1* autore) %s anno %d noleggio %s\n", iteratore->libro->titolo, iteratore->libro->autore[0], iteratore->libro->anno, iteratore->libro->prestito);
      iteratore = iteratore->next;
    }
  } else if (type == 3) { // LOG Query
    Libro *libri = (Libro *)data;

    fprintf(log, "%ld - Applica la query:\n", syscall(SYS_gettid));
    // DA FARE
  }
  fclose(log);
  UNLOCK(&qlock); // sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
  SIGNAL(&qcond);
}
