
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
      if (!input.eof())
        cout << numero << endl;
    } while (!input.eof());
    input.close();
  }

  output.open("./Tito_Laficara/esercizio1.txt");
  if (output.is_open())
    for (int i = 0; i < 10; i++)
      output << i << endl;
  output.close();

  output.open("./Tito_Laficara/esercizio1.txt", ios_base::app);
  if (output.is_open())
    for (int i = 9; i <= 20; i++)
      output << i << endl;
  output.close();

  return 0;
}