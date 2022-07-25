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
  // cout << array; <-- NO

  for (int i = 0; i < 10; i++) {
    cout << array1[i] << " ";
  }
  cout << endl << endl;

  // int array2[10] = array; // int array2[10] = {1, 2, 3, 4, ...}
  int array2[10];
  for (int i = 0; i < 10; i++) {
    array2[i] = array1[i];
  }
  return 0;
}