#include <cstdio>
#include <fstream>
#include <iostream>
using namespace std;

int main(void) {
  // INPUT
  ifstream input;
  // OUTPUT
  ofstream output;
  // MISTO
  fstream misto;

  // LETTURA
  input.open("./Tito_Laficara/esercizio1.txt");
  if (input.is_open()) {
    cout << "Aperto\n";
    int numero;
    do {
      input >> numero;
      if(!input.eof())
        cout << numero << endl;
    } while (!input.eof());
    input.close();
  }
  return 0;
}