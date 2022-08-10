#include <fstream>
#include <iostream>
using namespace std;

struct persone {
  string Nome;
  string Cognome;
  int Eta;
  float Altezza;
  string Citta;
};

void stampa(persone Vettore[], int N) {
  for (int i = 0; i < N; i++) {
    cout << Vettore[i].Nome << " " << Vettore[i].Cognome << " "
         << Vettore[i].Altezza << endl;
  }
}

int main(void) {

  persone leggi;
  persone Vettore[100];

  ifstream input;
  input.open("./Tito_Laficara/input_data1.txt");
  if (input.is_open()) {
    int i = 0;
    cout << "aperto\n";
    do {
      input >> leggi.Nome >> leggi.Cognome >> leggi.Eta >> leggi.Altezza >>
          leggi.Citta;
      if (!input.eof()) {
        // Tipo leggi = Tipo Vettore ==> Vettore[i] = leggi
        Vettore[i] = leggi;
        i++;
      }
    } while (!input.eof());
    input.close();
    stampa(Vettore, i);
  }
}