#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
  int numero;
  struct Nodo *next;
};

void push(struct Nodo **ptr, int val) {
  struct Nodo *ins = malloc(sizeof(struct Nodo));
  if (ins == NULL) exit(1);  // --> Memoria finita
  ins->numero = val;
  ins->next = NULL;

  if (*ptr == NULL) {
    *ptr = ins;
    return;
  }

  struct Nodo *tmp = *ptr;
  while (tmp->next != NULL) tmp = tmp->next;
  tmp->next = ins;
}

int length(struct Nodo *coda) {
  int length = 0;
  while (coda->next != NULL) {
    length++;
    coda = coda->next;
  }
  return length;
}

void print(struct Nodo *coda) {
  printf("Lista di %d element\n", length(coda));
  if(coda == NULL) return;

  while (coda->next != NULL) {
    printf("%d -> ", coda->numero);
    coda = coda->next;
  }
  printf("NULL\n");
}

int peek(struct Nodo *coda) {
  if (coda == NULL) return 0;
  return coda->numero;
}

void pop(struct Nodo **coda) {
  if (*coda == NULL) return;

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
    if (val % 2 == 1)
      push(&coda, val);
    else {
      if (val != 0) push(&coda, val / 2);
      if (val == 0) {
        if (peek(coda) % 2 == 0 || length(coda) > 4) {
          pop(&coda);
          contaZeri++;
        }
      }
    }
    if(val == 0 && contaZeri == 1) break;
    else contaZeri = 0;
  } while (1);
  print(coda);
  length(coda);
  return 0;
}
