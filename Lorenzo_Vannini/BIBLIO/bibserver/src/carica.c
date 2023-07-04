#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Libro
{
    char autore[5][50];
    char titolo[100];
    char editore[50];
    int anno;
    char nota[250];
    char collocazione[10];
    char luogo_pubblicazione[20];
    char descrizione_fisica[250];
};

typedef struct Libro Libro;

Libro *crea_libro(char *riga)
{
    Libro *in = (Libro *)malloc(sizeof(Libro));
    char *token;
    token = strtok(riga, ";");
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }
    printf("\n");
    return NULL;
}

int riempi_catalogo(char *file, Libro *catalogo)
{
    FILE *in_file = fopen(file, "r");
    char riga[1000];
    while (fgets(riga, 1000, in_file))
    {
        crea_libro(riga);
    }
    fclose(in_file);
}

int main(void)
{
    riempi_catalogo("../data/biblioteca_1.txt", NULL);
    return 0;
}