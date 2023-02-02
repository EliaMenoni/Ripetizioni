#include <iostream>
#include <string>
using namespace std;

int main(void){

    int numeri; // Senza virgola
    float numeri_virgola; // double numeri_virgola
    char carattere; // Caratteri \s \n \t
    String stringa;

    // Assegnamento
    numeri = 10;
    numeri_virgola = 10.5;
    carattere = 'A';
    stringa = "Parola";

    // Scrivi
    cout << numeri;
    cout << "Ciao";
    cout << "Quest è una " << stringa;
    cout << "Vado a capo\n";
    cout << "Vado a capo" << endl;

    // Leggi
    cin >> numeri;
    cin >> numeri >> carattere;

    return 0;
}