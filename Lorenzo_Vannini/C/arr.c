// Scrivere un programma che legge da standard input un array di interi, e,
// utilizzando una funzione, crea un secondo array con le somme delle coppie
// adiacenti di elementi, in ordine inverso. Il programma stampa il secondo
// array ottenuto.

// Per un array a di lunghezza n il risultato del calcolo è un array b di
// lunghezza n-1 dove b[i]=a[n-i-1]+a[n-i-2]. Nel caso in cui n<2 il programma
// non stampa niente.

// Il programma deve fare il controllo dell'input: se l'utente non inserisce un
// intero valide viene stampato il messaggio "Errore in input, inserisci un
// intero valido\n" e viene ripetuta la lettura da standard input.

// L'input contiene prima il numero di elementi dell'array e poi gli elementi
// stessi. In output l'array viene stampato un elemento per riga.
#include <stdio.h>
#include <stdlib.h>

int readInt() {
  int val;
  while (scanf("%d", &val) == 0) {
    printf("Errore in input, inserisci un intero valido\n");
    scanf("%*[^\n]\n");
  }
  return val;
}

void populate(int arr[], int dim){
    for (unsigned int i = 0; i < dim; i++)
    arr[i] = readInt();
}

int *calculate(int arr[], int dim, int* newDim){
    if(dim < 2) return NULL;
    *newDim = dim -1;
    int *newArr = malloc(sizeof(int)*(*newDim));
    for (int i = 0; i < *newDim; i++) 
        newArr[i] = arr[dim-i-1] + arr[dim-i-2];
    return newArr;
}

void print(int arr[], int dim){
    if(arr != NULL)
        for (unsigned int i = 0; i < dim; i++) printf("%d\n", arr[i]);
}

int main(int argc, char const *argv[]) {
  int dim = readInt();
  int arr[dim];
  populate(arr, dim);
  int newDim;
  int *res = calculate(arr, dim, &newDim);
  print(res, newDim);
  return 0;
}
