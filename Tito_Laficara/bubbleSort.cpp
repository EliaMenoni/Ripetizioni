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

void bubbleSort(int array[], int dim) {
  // Penso che l'array non sia ordinato
  bool ordinato = false;
  do {
    // Penso che l'array sia ordinato
    ordinato = true;
    // Ciclo fino al penultimo elemento non ordinato
    for (int i = 0; i < dim - 1; i++) {
      // Controllo che array[i] e array[i+1] siano ordinati
      if (array[i] > array[i + 1]) {
        // Altrimenti li ordino
        gira(array, i, i + 1);
        // Obbligo un nuovo ciclo
        ordinato = false;
      }
    }
    // Diminuisco la dimensione dell'array perchè l'ultimo elemento è ordianto
    // (il più grande)
    dim--;
    // continuo finchè non ho ordinato tutto
  } while (!ordinato);
}

// Stampa l'array
void stampa(int numeri[], int N) {
  for (int i = 0; i < N; i++) {
    cout << numeri[i] << " ";
  }
  cout << endl;
}

int main(void) {
  int array_da_ord[10];
  array(array_da_ord, 10);
  stampa(array_da_ord, 10);
  bubbleSort(array_da_ord, 10);
  stampa(array_da_ord, 10);
  return 0;
}