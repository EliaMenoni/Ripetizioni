#include <stdio.h>

#define ARRMAX 100

int clean_prime(int *a, int nelem) {
  int prime = 1;
  for (int i = 0; i < nelem; i++) {
    prime = 1;
    for (int j = 2; j < a[i] && prime; j++) {
      if (a[i] % j == 0) {
        prime = 0;
      }
    }
    if (!prime) {
      for (int j = i; j < nelem - 1; j++) {
        a[j] = a[j + 1];
      }
      nelem--;
      i--;
    }
  }
  return dim;
}

void stampa(int *numeri, int dim) {
  for (int i = 0; i < dim; i++) {
    printf("%d ", numeri[i]);
  }
  printf("\n");
}

int main(int argc, char const *argv[]) {
  int num;
  do {
    scanf("%d", &num);
  } while (num > 1000);

  int div[ARRMAX];
  int trovati = 0;
  for (int i = 1; i <= num - 1 && trovati < ARRMAX; i++) {
    if (num % i == 0) {
      div[trovati] = i;
      trovati++;
    }
  }
  stampa(div, trovati);
  trovati = clean_prime(div, trovati);
  stampa(div, trovati);
  return 0;
}
