#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

void array(int numeri[], int N) {
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    numeri[i] = rand() % 100 + 1;
  }
}

bool ricercaSec(int array[], int dim, int val) {
  for (int i = 0; i < dim; i++) {
    if (array[i] == val)
      return true;
  }
  return false;
}

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

void insertionSort(int array[], int dim) {
  int val;
  // array[0] -> Banalmente ordinato
  // Ciclo per ogni lemento dell'array
  for (int i = 1; i < dim; i++) {
    // Salvo il valore dell'elemento corrente
    val = array[i];
    // Mi sposto verso sinistra finchè
    for (int j = i - 1; j >= 0; j--) {
      // Se l'elemento j precedente è > lo scambio e passo al precedente ancora
      if (array[j] > val)
        gira(array, j, j + 1);
      else
        break;
    }
  }
}

void stampa(int numeri[], int N) {
  for (int i = 0; i < N; i++) {
    cout << numeri[i] << " ";
  }
  cout << endl;
}

int main(void) {
  // <tipo> <nome>[<dimensione>];
  int array1[10]; // Indici 0 - 9

  // Lettura
  // <nome>[<indice da leggere>];
  array1[0];

  // Scrittura
  // <nome>[<indice da scrivere>] = <val>;
  array1[0] = 1;

  // Stampa
  // cout << array;

  for (int i = 0; i < 10; i++) {
    cout << array1[i] << " ";
  }
  cout << endl << endl;

  // int array2[10] = array; // int array2[10] = {1, 2, 3, 4, ...}
  int array2[10];
  for (int i = 0; i < 10; i++) {
    array2[i] = array1[i];
  }

  int array_da_ord[10];
  array(array_da_ord, 10);
  stampa(array_da_ord, 10);
  selectionSort(array_da_ord, 10);
  stampa(array_da_ord, 10);

  cout << endl << endl;

  array(array_da_ord, 10);
  stampa(array_da_ord, 10);
  bubbleSort(array_da_ord, 10);
  stampa(array_da_ord, 10);

  cout << endl << endl;

  array(array_da_ord, 10);
  stampa(array_da_ord, 10);
  insertionSort(array_da_ord, 10);
  stampa(array_da_ord, 10);
  return 0;
}