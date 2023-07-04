// Scrivere un programma che legge da standard input una matrice di interi e
// stampa la sua trasposta. Il programma deve utilizzare una funzione per
// costruire la trasposta.

// La trasposta di una matrice inverte le righe e le colonne (le colonne
// diventano le righe e le righe diventano le colonne). Data una matrice a, la
// trasposta a^T e definita come a^T [i][j] = a[j][i].

// L'input contiene prima il numero di righe e di colonne, poi gli elementi
// della matrice riga per riga.

#include <stdio.h>
#include <stdlib.h>

void trasposta(int **matr, int rows, int cols) {
  int tmp;
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = i; j < cols; j++) {
      tmp = matr[i][j];
      matr[i][j] = matr[j][i];
      matr[j][i] = tmp;
    }
  }
}

void print(int **matr, int rows, int cols) {
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      printf("%d\t", matr[i][j]);
    }
    printf("\n");
  }
}

int main(void) {
  int rows, cols;
  scanf("%d %d", &rows, &cols);
  int **matr = (int **)malloc(sizeof(int *) * rows);
  for (unsigned int i = 0; i < rows; i++)
    matr[i] = (int *)malloc(sizeof(int) * cols);
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      scanf("%d", &matr[i][j]);
    }
  }
  printf("\n");
  print(matr, rows, cols);
  printf("\n");
  trasposta(matr, rows, cols);
  print(matr, rows, cols);
  return 0;
}