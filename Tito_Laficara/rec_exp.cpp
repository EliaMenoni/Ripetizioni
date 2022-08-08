#include <iostream>
using namespace std;

// n! = n*(n-1)!
long int rec_fatt(int n){
    if(n <= 1) return 1;         // Caso Base / Condizione di uscita
    return n*rec_fatt(n-1);   // Chiamata ricorsiva
}

// n! = 1 * 2 * ... * n
long int fatt(int f) {
  int r = f;
  for (int i = 1; i<r; i++) {
    f = f * i;
  }
  /*
    5 * 1 = 5
    5 * 2 = 10
    10 * 3 =
    30 * 4 =

    1 * 2 * 3 * 4 * 5

  */
  return f;
}

int main(void) {
  long int res = fatt(0);
  cout << res << endl;
  res = rec_fatt(0);
  cout << res << endl;
  return 0;
}