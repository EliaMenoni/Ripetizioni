// Scrivere un programma in C che implementa un albero binario di ricerca
// contenente numeri interi. Definire la struttura di dati necessaria.
// Implementare le seguente funzioni:

// -insert - prende un albero binario di ricerca e inserisce un nuovo elemento
// -printAscending - prende un albero binario di ricerca e stampa gli elementi
// in ordine crescente, visitando l'albero tutti sulla stessa riga separati da
// spazio. -find - prende in albero e un elemento, e restituisce 1 se l'elemento
// esiste, 0 altrimenti Implementare una funzione main che esegue le operazioni:
// -legge da standard input un numero intero n seguito da n interi, e li
// inserisce in un nuovo albero. -stampa gli elementi in ordine crescente -legge
// un altro numero intero e controlla se esiste nell'albero, stampando True se
// trovato o False altrimenti.
#include <stdio.h>
#include <stdlib.h>

struct Node {
  int val;
  struct Node *SX, *DX;
};

void printAscending(struct Node *tree) {
  if (tree == NULL) return;
  printAscending(tree->SX);
  printf("%d ", tree->val);
  printAscending(tree->DX);
}

int find(struct Node *tree, int val) {
  if (tree == NULL) return 0;
  
  int found = 0;
  while (!found) {
    if (tree->val == val) return 1;
    if (tree->val < val) {
      if (tree->DX != NULL) {
        tree = tree->DX;
      } else {
        return 0;
      }
    } else {
      if (tree->SX != NULL) {
        tree = tree->SX;
      } else {
        return 0;
      }
    }
  }
}

void insert(struct Node **tree, int val) {
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  newNode->val = val;
  newNode->DX = NULL;
  newNode->SX = NULL;

  if (*tree == NULL) {
    *tree = newNode;
    return;
  }

  struct Node *tmp = *tree;
  int done = 0;
  while (!done) {
    if (tmp->val < val) {
      if (tmp->DX != NULL) {
        tmp = tmp->DX;
      } else {
        tmp->DX = newNode;
        done = 1;
      }
    } else {
      if (tmp->SX != NULL) {
        tmp = tmp->SX;
      } else {
        tmp->SX = newNode;
        done = 1;
      }
    }
  }
}

void dataManage(struct Node** tree){
  int toRead;
  scanf("%d", &toRead);
  int toInsert;
  for (unsigned int i = 0; i < toRead; i++){
    scanf("%d", &toInsert);
    insert(tree, toInsert);
  }
  printAscending(*tree);
  printf("\n");
  int toCheck;
  scanf("%d", &toCheck);
  printf(find(*tree, toCheck) ? "True\n" : "False\n");
}

int main(void) {
  struct Node *tree = NULL;
  dataManage(&tree);
  return 0;
}