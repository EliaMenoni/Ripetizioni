#include <iostream>
using namespace std;

int main(void){
    int N,somma=0;
    cout<<"Inserisci N"<<endl;
    cin>>N;
    if(N>0){
    for(int i=0;i<N;i++){
        int x;
        cout<<"inserisci x"<<i<<endl;
        cin>>x;
        somma=somma+x*x;
    }
    cout<<somma<<endl;
    }else{
        cout<<"errore"<<endl;
    }
    return 0;
}