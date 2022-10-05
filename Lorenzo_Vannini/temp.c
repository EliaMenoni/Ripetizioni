#include <stdio.h>
#include <stdlib.h>

void trasposta(int righe, int colonne, int matrix[][colonne]);
void stampa(int colonne, int righe, int matrix[][colonne]);

int readInt() {
  int x;
  while (scanf("%d", &x) == 0) {
    printf("Errore in input, inserici un intero valido\n");
    scanf("%*[^\n]\n");
  }
  return x;
}

int main() {
  int colonne, righe;

  colonne = readInt();
  righe = readInt();
  int matrix[righe][colonne];

  trasposta(righe, colonne, matrix);
  stampa(righe, colonne, matrix);

  return 0;
}

void trasposta(int righe, int colonne, int matrix[][colonne]) {
  int i, j, tmp;
  for (i = 0; i < colonne; i++)
    for (j = 0; j < righe; j++) {
      tmp = readInt();
      matrix[j][i] = tmp;
    }
}

void stampa(int righe, int colonne, int matrix[][colonne]) {
  int i, j;
  for (i = 0; i < righe; i++) {
    for (j = 0; j < colonne; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}