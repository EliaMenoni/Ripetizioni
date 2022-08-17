#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

struct libro {
  int codice;
  string titolo;
  string autori[8];
  string argomento;
  int anno;
  int numCatalogo;
};

struct argomento {
  string argomento;
  int numLibri;
};

void cercaTitolo(libro BIBLIO[], int n, string titolo) {
  for (int i = 0; i < n; i++) {
    if (BIBLIO[i].titolo == titolo) {
      for (int j = 0; j < 8; j++) {
        if (BIBLIO[i].autori[j] != "")
          cout << BIBLIO[i].autori[j] << endl;
      }
    }
  }
}

int contaLibri(libro BIBLIO[], int n, int anno) {
  int libriTrovati = 0;
  for (int i = 0; i < n; i++) {
    if (BIBLIO[i].anno == anno)
      libriTrovati++;
  }
  return libriTrovati;
}

int trasferimento(libro BIBLIO[], argomento argomenti[], int n) {
  int argTrovati = 0;
  for (int i = 0; i < n; i++) {
    argomenti[i].argomento = "";
  }
  for (int i = 0; i < n; i++) {
    argomenti[i].argomento = BIBLIO[i].argomento;
    argomenti[i].numLibri = 1;
  }
  for (int i = 0; i < n; i++) {
    if (argomenti[i].argomento != "") {
      for (int j = i + 1; j < n; j++) {
        if (argomenti[i].argomento == argomenti[j].argomento) {
          argomenti[i].numLibri += argomenti[j].numLibri;
          argomenti[j].argomento = "";
        }
      }
      argTrovati++;
    } else {
      for (int j = i; j < n - 1; j++) {
        argomenti[j] = argomenti[j + 1];
      }
      n--;
    }
  }
  return argTrovati;
}

void popolaLibri(libro BIBLIO[], int n) {
  srand(time(NULL));
  string supp;
  for (int i = 0; i < n; i++) {
    BIBLIO[i].codice = rand() % 1000 + 1;
    // supp -> string
    // str -> string
    // supp + str = "ciao" + "Tito" = "ciaoTito"
    supp = "Titolo";
    BIBLIO[i].titolo = supp + to_string(i);
    supp = "Argmento";
    BIBLIO[i].argomento = supp + to_string((rand() % 5 + 1));
    BIBLIO[i].anno = rand() % 2001;
    BIBLIO[i].numCatalogo = i;
    for (int j = 0; j < 8; j++) {
      BIBLIO[i].autori[j] = "";
    }
    int nAutori = rand() % 8 + 1;
    supp = "Autore";
    for (int j = 0; j < nAutori; j++) {
      BIBLIO[i].autori[j] = supp + to_string(rand() % 5 + 1);
    }
  }
}

void stampa(libro BIBLIO[], int n){
    for (int i = 0; i < n; i++) {
        cout << BIBLIO[i].titolo << " " << BIBLIO[i].anno << endl;
        cout << BIBLIO[i].argomento << endl;
        for (int j = 0; j < 8; j++) {
         cout << BIBLIO[i].autori[j] << ", ";
        }
        cout << "\n\n";
    }
}

int main(void) {
  int N = 10;
  libro BIBLIO[N];
  // Popolo la libreria
  popolaLibri(BIBLIO, N);
  //stampa(BIBLIO, N);
  // Cerco per Titolo
  string titoloDaCercare;
  cout << "Titolo da cercare: ";
  cin >> titoloDaCercare;
  cercaTitolo(BIBLIO, N, titoloDaCercare);
  // Conto Libri dato un anno
  int annoDaCercare;
  cout << "Anno da cercare: ";
  cin >> annoDaCercare;
  int libriContati = contaLibri(BIBLIO, N, annoDaCercare);
  cout << "Libri contati per l'anno " << annoDaCercare << ": " << libriContati
       << endl;
  argomento ARG[N];
  int args = trasferimento(BIBLIO, ARG, N);
  for (int i = 0; i < args; i++) {
    cout << ARG[i].argomento << ": " << ARG[i].numLibri<<endl;
  }
  return 0;
}