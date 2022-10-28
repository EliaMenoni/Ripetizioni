#include <iostream>
using namespace std;

int main(){
    int squadre;

    do{
        cin>>squadre;
        if(squadre <= 0 || squadre > 30) cout << "Errore\n";
    }while(squadre <= 0 || squadre > 30);

    int min = 10000000000;
    int vittorie;
    int pareggi;

    for(int i = 0; i < squadre; i++){
        cin >> vittorie >> pareggi;
        
        if(vittorie * 3 + pareggi >= 15)
            cout << "Passata" << endl;

        if(min > vittorie * 3 + pareggi)
            min = vittorie * 3 + pareggi;
    }

    cout << "MIN: " << min << endl;
    return 0;
}