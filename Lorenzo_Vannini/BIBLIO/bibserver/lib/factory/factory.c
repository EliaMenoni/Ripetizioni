#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

#include "../comprot/comprot.h"
#include "../libro/libro.h"
#include "../unboundedqueue/unboundedqueue.h"
#include "../unboundedqueue/util.h"
#include "factory.h"

volatile int terminazione = 0;
char file_name[250];

void write_log(void *data, int type)
{                               // apre un file e scrive una stringa data da noi, dobbiamo sfruttare lock e unlock perchè non possiamo accedere a un file con più thread contemporaneamente
  static pthread_mutex_t qlock; // per rendere il semaforo disponibile a tutti i thread, anzichè passarlo come argomento, lo abbiamo reso statico
  static pthread_cond_t qcond;  // viene creato all'avvio dell'esecuzione del programma e rimane disponibile dentro la funzione write_log
  LOCK(&qlock);
  FILE *log;
  log = fopen("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibserver/logs/requests.log", "a+"); // apro il file con "a+" perchè permette di aprire il file in scrittura senza cancellare il contenuto e posizionandosi alla fine.
  if (log == NULL)                                                                                 // mentre con "w" il contenuto veniva cancellato ogni volta, stampando solo l'ultima esecuzione
    exit(1);

  if (type == 0)
  { // Stringa
    char *text = (char *)data;

    fprintf(log, "%s\n", text); // scrivo nel file di log il testo dato da noi e mando a capo, poi chiudo il file
  }
  else if (type == 1)
  { // Risultato Operazione Query
    Nodo *libri = (Nodo *)data;

    if (libri == NULL)
    {
      fprintf(log, "QUERY 0\n");
      fclose(log);
      UNLOCK(&qlock); // sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
      SIGNAL(&qcond);
      return;
    }

    int n_libri = 0;
    Nodo *iteratore = libri;
    while (iteratore != NULL)
    {
      n_libri++;
      iteratore = iteratore->next;
      // da aggiungere i dati
    }

    iteratore = libri;
    char *output;
    while (iteratore)
    {
      output = libro_toString(iteratore->libro);
      fprintf(log, "%s\n\n", output);
      free(output);
      if (iteratore->next != NULL)
        fprintf(log, "\n");
      iteratore = iteratore->next;
    }
    fprintf(log, "QUERY %d\n", n_libri);
  }
  else if (type == 2)
  { // Risultato Operazione Noleggio
    Nodo *libri = (Nodo *)data;

    if (libri == NULL)
    {
      fprintf(log, "LOAN 0\n");
      fclose(log);
      UNLOCK(&qlock); // sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
      SIGNAL(&qcond);
      return;
    }

    int n_libri = 0;
    Nodo *iteratore = libri;
    while (iteratore != NULL)
    {
      n_libri++;
      iteratore = iteratore->next;
      // da aggiungere i dati
    }

    iteratore = libri;
    char *output;
    while (iteratore)
    {
      output = libro_toString(iteratore->libro);
      fprintf(log, "%s\n\n", output);
      free(output);
      if (iteratore->next != NULL)
        fprintf(log, "\n");
      iteratore = iteratore->next;
    }
    fprintf(log, "LOAN %d\n", n_libri);
  }
  fclose(log);
  UNLOCK(&qlock); // sblocco il semaforo e segnalo a tutti i thread in attesa che è stato sbloccato
  SIGNAL(&qcond);
}

void termina_processo(int signum)
{
  write_log("Terminazione Server", 0);
  terminazione = 1;
}

DataPool *server_read(int socket, Nodo *libreria)
{
  Packet incoming;
  read(socket, &incoming, sizeof(Packet));
  Libro *filter = crea_libro_da_stringa(incoming.data);

  DataPool *outgoin = malloc(sizeof(DataPool));

  memset(outgoin, 0, sizeof(outgoin));
  if (outgoin == NULL)
    exit(1);

  if (incoming.type == MSG_LOAN)
    outgoin->noleggio = 1;
  else
    outgoin->noleggio = 0;

  outgoin->connection_number = socket;
  outgoin->filtro = filter;
  outgoin->radice_libreria = libreria;

  return outgoin;
}

void *worker(void *arg)
{
  pid_t tid = syscall(SYS_gettid); // prende l'id del sottoprocesso corrente
  char *out_buffer = malloc(250);  // il buffer in cui mettiamo la stringa formattata da stampare nel log

  memset(out_buffer, '\0', sizeof(out_buffer));
  if (out_buffer == NULL)
    exit(1);

  Queue_t *coda = (Queue_t *)arg;

  printf("Worker %d avviato\n", tid);
  sprintf(out_buffer, "Worker %d avviato", tid);
  write_log(out_buffer, 0);

  while (!terminazione || length(coda) > 0)
  {
    if (length(coda) == 0)
    {
      // printf("Worker %d e' in attesa di richieste\n", tid);
      continue;
    }

    DataPool *richiesta = (DataPool *)pop(coda);

    printf("Worker %d serve una richiesta\n", tid);
    Nodo *libreria = richiesta->radice_libreria;
    Nodo *risultato = ricerca_libri(libreria, richiesta->filtro);
    Nodo *out = NULL;
    Nodo *out_cursore = NULL;

    if (richiesta->noleggio == 1)
    {
      sprintf(out_buffer, "%d - servo richiesta noleggio", tid); // fa il lavoro della printf ma anzichè stamparla la mette in un buffer
      write_log(out_buffer, 0);

      Nodo *cursore = risultato;
      aggiorna_scadenze_prestiti(libreria);

      while (cursore != NULL)
      {
        if (noleggia(cursore->libro))
        {
          if (out == NULL)
          {
            out = malloc(sizeof(Nodo));

            memset(out, 0, sizeof(out));
            if (out == NULL)
              exit(1);
            out_cursore = out;
            out->libro = cursore->libro;
            out->next = NULL;
          }
          else
          {
            out_cursore->next = malloc(sizeof(Nodo));

            memset(out_cursore->next, 0, sizeof(out_cursore->next));
            if (out_cursore->next == NULL)
              exit(1);
            out_cursore = out_cursore->next;
            out_cursore->libro = cursore->libro;
            out_cursore->next = NULL;
          }
        }
        cursore = cursore->next;
      }
      write_log(out, 2);
    }
    else
    {
      out = risultato;
      write_log(out, 1);
    }

    out_cursore = out;
    Packet risposta;
    if (out_cursore == NULL)
    {
      risposta.type = MSG_NO;
      risposta.length = 0;

      send(richiesta->connection_number, &risposta, sizeof(risposta), 0);
    }
    while (out_cursore)
    {
      // stampa_libro(cursore->libro);
      char *risposta_str = libro_toString(out_cursore->libro);
      strcpy(risposta.data, risposta_str);
      strcat(risposta.data, "\0");
      // printf("%s\n", risposta);
      free(risposta_str);
      risposta_str = NULL;

      send(richiesta->connection_number, &risposta, sizeof(risposta), 0);
      out_cursore = out_cursore->next;
    }
    out_cursore = risultato;
    Nodo *prev;
    while (out_cursore)
    {
      prev = out_cursore;
      out_cursore = out_cursore->next;
      prev->libro = NULL;
      free(prev);
      prev = NULL;
    }
    if (richiesta->noleggio)
    {
      out_cursore = out;
      while (out_cursore)
      {
        prev = out_cursore;
        out_cursore = out_cursore->next;
        free(prev);
        prev = NULL;
      }
    }
    out = NULL;

    Packet stream_end;
    stream_end.type = MSG_NO;
    send(richiesta->connection_number, &stream_end, sizeof(Packet), 0);
    close(richiesta->connection_number);
    // delete_libreria(risultato);
    free(richiesta->filtro);
    richiesta->filtro = NULL;
    free(richiesta);
    richiesta = NULL;
  }
  free(out_buffer);
  out_buffer = NULL;
}
