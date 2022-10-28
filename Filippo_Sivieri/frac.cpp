#include <iostream>
using namespace std;

int main(){
    int N, num, den;
    do{
        cin>>N;
        if(N <= 0 || N >= 20) cout << "Errore\n";
    }while(N <= 0 || N >= 20);
    for(int i = 0; i < N; i++){
        cin >> num >> den;
        if(num % den == 0) cout << "Apparenti\n";
        else {
            if(num / den >= 1)
                cout << "Impropria\n";
            else
                cout << "Propria\n";
        }
    }
    return 0;
}