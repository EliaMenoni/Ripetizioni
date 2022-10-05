#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRMAX 100

struct Persona {
  char nome[ARRMAX];
  int oggetti;
  int valore_ogg;
};

int main(int argc, char const *argv[]) {
  srand(0);
  struct Persona persone[50];

  char temp[ARRMAX];
  for (int i = 0; i < 50; i++) {
    sprintf(temp, "persona - %d", rand() % 100 + 1);
    strcpy(persone[i].nome, temp);
    persone[i].oggetti = rand() % 100 + 1;
    int val = 0;
    do {
      val = rand() % 100000 + 1;
    } while (val % 100 != 0);
    persone[i].valore_ogg = val;
  }

  int max = 0;
  for (int i = 0; i < 50; i++) {
    persone[i].nome[0] = persone[i].nome[0] - 32;
    if(persone[i].oggetti * persone[i].valore_ogg > persone[max].oggetti * persone[max].valore_ogg)
    max = i;
  }

  printf("%s\n", persone[max].nome);

  return 0;
}
