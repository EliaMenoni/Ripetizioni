#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// Popola l'array con numeri casuali da 1 a 100
void array(int numeri[], int N) {
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    numeri[i] = rand() % 10 + 1;
  }
}

bool ricercaBin(int array[], int val, int inizio, int fine) {
  // Condizione di uscita
  if (inizio == fine)
    return array[inizio] == val;

  // Mi sposto al centro
  int mid = (inizio + fine) / 2;
  // Guardo se l'elemento al centro è quello che mi serve
  if (array[mid] == val)
    return true;
  // Se è < di quelo che cerco mi sposto a DX
  if (array[mid] < val)
    return ricercaBin(array, val, mid + 1, fine);
  // Se è > di quelo che cerco mi sposto a SX
  if (array[mid] > val)
    return ricercaBin(array, val, inizio, mid - 1);

  return false;
}

void merge(int array1[], int dim1, int array2[], int dim2, int merged[],
           int dimMerge) {
  int ind1 = 0, ind2 = 0;
  // Per ogni elemento del vettore di merge
  for (int i = 0; i < dimMerge; i++) {
    // Se finisco array1
    if (ind1 > dim1) {
      // Consumo array2
      merged[i] = array2[ind2];
      ind2++;
    }
    // Se finisco arra2
    if (ind2 > dim2) {
      // Consumo array1
      merged[i] = array1[ind1];
      ind1++;
    }
    // Altromenti cerco il valore minore tra i due correnti in array1 e array2
    if (array1[ind1] < array2[ind2]) {
      // Se il minore si trov in array1 lo consumo
      merged[i] = array1[ind1];
      ind1++;
    } else {
      // Se il minore si trova in array2 lo consumo
      merged[i] = array2[ind2];
      ind2++;
    }
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
  int dim1 = 10;
  int array1[dim1];
  int dim2 = 15;
  int array2[dim2];

  int dim3 = dim1 + dim2;
  int merged[dim3];

  array(array1, dim1);
  array(array2, dim2);

  bubbleSort(array1, dim1);
  bubbleSort(array2, dim2);
  stampa(array1, dim1);
  stampa(array2, dim2);

  merge(array1, dim1, array2, dim2, merged, dim3);

  stampa(merged, dim3);
  return 0;
}