 #include <iostream>
using namespace std;

int main(void){
    int N,somma=0;
    cout<<"Inserisci N"<<endl;
    cin>>N;
    if(N>0){
    for(int i=0;i<N;i++){
        somma=somma+i+1;
    }
    cout<<somma<<endl;
    }else{
        cout<<"errore"<<endl;
    }
    return 0;
}