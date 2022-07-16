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

int numeromassimo(int numeri[], int N) {
  int nmax = numeri[0];
  for (int i = 1; i < N; i++) {
    if (nmax < numeri[i]) {
      nmax = numeri[i];
    }
  }
  return nmax;
}

void stampa(int numeri[], int N) {
  for (int i = 0; i < N; i++) {
    cout << numeri[i] << " ";
  }
  cout << endl;
}

int main() {
  int N = 20;
  int numeri[N];
  stampa(numeri, N);
  array(numeri, N);
  stampa(numeri, N);
  cout << numeromassimo(numeri, N);
  return 0;
}