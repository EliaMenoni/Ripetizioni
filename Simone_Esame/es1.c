#include <stdio.h>

int potenza(int base, int exp) {
  if (exp == 0)
    return 1;
  int tmp = base;
  for (int i = 1; i < exp; i++) {
    tmp *= base;
  }
  return tmp;
}

int main(int argc, char const *argv[]) {
  int x, y;

  do {
    scanf("%d", &x);
  } while (x < 0);

  do {
    scanf("%d", &y);
  } while (y <= 0);

  int esponente = 0;
  while (potenza(y, esponente + 1) <= x) {
    esponente ++;
  }

  int resto = x - potenza(y, esponente);

  printf("%d %d\n", esponente, resto);

  return 0;
}
