#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

struct Ora {
  int ore;
  int min;
  int sec;
};

struct Data {
  int giorno;
  int mese;
  int anno;

  Ora ora;
};

// Definizione strutta Persona
struct Persona {
  string nome;
  string cognome;
  Data bDay;
  unsigned int eta;

  float altezza;
};

int main(void) {
  Persona piero;

  piero.nome = "Piero";
  piero.cognome = "Franco";
  piero.eta = 25;
  piero.altezza = 1.9;

  piero.bDay.giorno = 1;
  piero.bDay.mese = 2;
  piero.bDay.anno = 2000;

  piero.bDay.ora.ore = 10;

  Persona gente[20];
  gente[15].nome = "Titò";
  gente[15].eta = 17;
  gente[15].bDay.anno = 2005;

  /*
  for(int i = 0; i < 20; i++){
      cout << gente[i].nome << endl;
      cout << gente[i].eta << endl;
  }
  */

  Persona tito = gente[15];
  cout << tito.nome << endl;
  cout << tito.eta << endl;
  cout << tito.bDay.anno << endl;
}