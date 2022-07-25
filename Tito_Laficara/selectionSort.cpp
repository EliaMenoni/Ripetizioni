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

void selectionSort(int array[], int dim) {
  int min;
  // Per ogni elemento di array
  for (int i = 0; i < dim - 1; i++) {
    // Salvo l'indice del possibile elemento minimo
    min = i;
    // Controllo array[min] sia effettivamente il numero più piccolo
    for (int j = i + 1; j < dim; j++) {
      // altrimenti salvo il nuovo indice
      if (array[j] < array[min])
        min = j;
    }
    // Giro la cella corrente con quella che contiene l'elemento minore
    if (i != min)
      gira(array, i, min);
  }
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
  selectionSort(array_da_ord, 10);
  stampa(array_da_ord, 10);
  return 0;
}