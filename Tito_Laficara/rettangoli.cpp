#include <iostream>
using namespace std;

int main(void) {
  int larghezza, altezza;
  cin >> larghezza >> altezza;

  // Per ogni riga
  for (int i = 0; i < altezza; i++) {
    // Per ogni colonna
    for (int j = 0; j < larghezza; j++) {
      // Se è la prima o l'ultima riga stampo solo asterischi
      if (i == 0 or i == altezza - 1)
        cout << "*";
      else {
        // Per le righe interne stampo asterischi solo per la prima e l'ultima
        // colonna
        if (j == 0 or j == larghezza - 1)
          cout << "*";
        // Tutto il resto è uno spazio
        else
          cout << " ";
      }
    }
    cout << endl;
  }
}