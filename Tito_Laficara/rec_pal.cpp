#include <iostream>
using namespace std;

int conta (char str[]){
int conteggio = 0;
    while (str[conteggio]){
        conteggio ++;
    }
    return conteggio;
}

bool palindroma (char str[]){
    if (str[0] == '*' && conta (str) == 1){
        return true;
    }

}

int main(void) {

  return 0;
}