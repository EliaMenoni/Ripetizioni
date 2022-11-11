#include <iostream>
using namespace std;

int main(void){

    // cout << "***\n*X*\n*X*\n*X*\n*X*\n*X*\n*X*\n*X*\n*X*\n***";

    // Righe
    for(int i = 0; i < 10; i++){
        // if(i != 0 && i != 9) {
        //     cout << "*X*";
        // } else {
        //     cout << "***";
        // }
        // Colonne
        for(int j = 0; j < 3; j++){
            if(i == 0 || i == 9){
                cout << "*";
            } else {
                if(j == 0 || j == 2){
                    cout << "*";
                } else {
                    cout << "X";
                }
            }
        }
        cout << "\n";
    }

    return 0;
}