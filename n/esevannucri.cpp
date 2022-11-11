#include <iostream>
#include <fstream>
using namespace std;

int main(void){
    //File
    fstream file;
    //Leggo il file
    file.open("./temperature.txt", ios::in)
    int DIM;
    file >> DIM;

    float temerature[DIM];
    
    //Chiudo il file
    //Faccio i miei conti
    //Scrivo il file
    //Chiudo il file

    return 0;
}