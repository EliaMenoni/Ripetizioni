// Implementare, utilizzando le liste concatenate, la struttura dati Coda
// (FIFO). Devono essere implementate delle funzioni che permettono le seguente
// operazioni:

// -push - inserisce un elemento nella coda
// -pop - rimuove il primo elemento dalla coda
// -peek - restituisce il valore del primo elemento della coda senza rimuoverlo
// -length - calcola la lunghezza della coda in modo ricorsivo
// -print - stampa gli elementi della coda, iniziando con il primo elemento
// (vedi esempio) in modo ricorsivo

// Scrivere una funzione main che legge da standard input una serie di interi e,
// utilizzando le funzioni definite, esegue le seguenti operazioni:

// -Se il numero è dispari viene inserito nella coda
// -Se il numero è pari diverso da 0, viene inserito il numero diviso per 2
// -Se il numero è 0, viene eliminato un elemento dalla coda, solo se l'elemento
// è pari o la lista contiene più di 4 elementi. -Il programma si ferma quando
// si leggono due 0 di seguito. Viene stampato il contenuto della coda e la
// lunghezza

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
  int numero;
  struct Nodo *next;
};

void push(struct Nodo **ptr, int val) {
  struct Nodo *ins = malloc(sizeof(struct Nodo));
  if (ins == NULL)
    exit(1); // --> Memoria finita
  ins->numero = val;
  ins->next = NULL;

  if (*ptr == NULL) {
    *ptr = ins;
    return;
  }

  struct Nodo *tmp = *ptr;
  while (tmp->next != NULL)
    tmp = tmp->next;
  tmp->next = ins;
}

int length(struct Nodo *coda) {
  if (coda == NULL)
    return 0;
  int length = 1;
  while (coda->next != NULL) {
    length++;
    coda = coda->next;
  }
  return length;
}

// void print(struct Nodo *coda) {
//   printf("Lista di %d element\n", length(coda));
//   if (coda == NULL) return;

//   while (coda->next != NULL) {
//     printf("%d\n", coda->numero);
//     coda = coda->next;
//   }
// }

void print(struct Nodo *coda) {
  if (coda == NULL)
    return;
  printf("%d\n", coda->numero);
  print(coda->next);
}

int peek(struct Nodo *coda) {
  if (coda == NULL)
    return 0;
  return coda->numero;
}

void pop(struct Nodo **coda) {
  if (*coda == NULL)
    return;

  struct Nodo *x = *coda;
  *coda = x->next;
  free(x);
}

int main() {
  struct Nodo *coda = NULL;
  int val;
  int contaZeri = 0;
  do {
    scanf("%d", &val);
    if (abs(val) % 2 == 1)
      push(&coda, val);
    else {
      if (val != 0)
        push(&coda, val / 2);
      if (val == 0) {
        if (peek(coda) % 2 == 0 || length(coda) > 4) {
          pop(&coda);
        }
      }
    }
    if (val == 0 && contaZeri == 1)
      break;
    else
      contaZeri = val == 0 ? contaZeri + 1 : 0;
  } while (1);
  printf("Lista di %d elementi\n", length(coda));
  print(coda);
  return 0;
}
