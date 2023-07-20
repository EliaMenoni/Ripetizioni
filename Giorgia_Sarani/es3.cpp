/*
    Un'azienda produce ingranaggi per motori di differente grandezza (piccolo, medio, grande).
    Vuole realizzare un programma che fornisca un preventivo per la quantità di prezzi richiesti.

    Piccolo 35€
    Medio 40€
    Grande 45€
*/
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]){
    // 10 pezzi
    // Piccolo
    // 10 (pezzi) x 35 (prezzo Piccolo) = 350€ (Totale)


    // Codice
    int pezzi;
    int tipo;

    // Leggo Numero Pezzi
    cout << "Numero Pezzi: "; // Stampa (Codice -> Utente)
    cin >> pezzi;             // Lettura (Utente -> Codice)

    // Leggo Tipo Pezzi
    cout << "Tipo Pezzi:" << endl << "\t1 - Piccolo (35€)\n\t2 - Medio (40€)\n\t3 - Grande (45€)\n\nTipologia scelta: ";
    /*
        \n -> A capo equivalente a endl
        \t -> Tab
    */
    cin >> tipo;

    int prezzo_tipo = 0;

    if(tipo == 1) {
        prezzo_tipo = 35;
    } else if(tipo == 2) {
        prezzo_tipo = 40;
    } else if(tipo == 3) {
        prezzo_tipo = 45;
    }

    // switch (tipo) {
    //     case 1:
    //         prezzo_tipo = 35;
    //         break;
    //     case 2:
    //         prezzo_tipo = 40;
    //         break;
    //     case 3:
    //         prezzo_tipo = 45;
    //         break;
    // }

    // Calcolo preventivo
    int preventivo = 0;             // int preventivo = pezzi * prezzo_tipo;
    preventivo = pezzi * prezzo_tipo;    

    // Stampo il risultato
    cout << "Preventivo: " << preventivo << "€\n";

    return 0;
}
