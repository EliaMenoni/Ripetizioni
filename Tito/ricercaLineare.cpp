#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// Popola l'array con numeri casuali da 1 a 100
void array(int numeri[], int N) {
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    numeri[i] = rand() % 100 + 1;
  }
}

// Scambia gli elementi di indice i e j nell'array
void gira(int array[], int i, int j) {
  int swap = array[i];
  array[i] = array[j];
  array[j] = swap;
}

bool ricercaSec(int array[], int dim, int val) {
  for (int i = 0; i < dim; i++) {
    if (array[i] == val)
      return true;
  }
  return false;
}

// Stampa l'array
void stampa(int numeri[], int N) {
  for (int i = 0; i < N; i++) {
    cout << numeri[i] << " ";
  }
  cout << endl;
}

int main(void) {
  int array_da_cercare[10];
  array(array_da_cercare, 10);
  stampa(array_da_cercare, 10);
  cout << ricercaSec(array_da_cercare, 10, 25) << endl;
  return 0;
}