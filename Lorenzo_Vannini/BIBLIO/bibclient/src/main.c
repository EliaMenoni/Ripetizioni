#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "../../bibserver/lib/libro/libro.h"

#define PORT 8000

int main(int argc, char const *argv[])
{
    printf("POPOLO RICHIESTA DI PROVA\n");
    Libro filtro;
    filtro.numero_autori = 1;
    strcpy(filtro.autore[0], "");
    strcpy(filtro.titolo, "arte della");
    strcpy(filtro.editore, "");
    filtro.anno = -1;
    strcpy(filtro.collocazione, "");
    strcpy(filtro.descrizione_fisica, "");
    strcpy(filtro.prestito, "");

    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    int noleggio = 1;
    send(client_fd, &filtro, sizeof(Libro), 0);
    send(client_fd, &noleggio, sizeof(int), 0);

    Libro ricevuto;
    do
    {
        valread = read(client_fd, &ricevuto, sizeof(Libro));
        if (ricevuto.numero_autori != -1)
            stampa_libro(&ricevuto);
    } while (ricevuto.numero_autori != -1);

    // closing the connected socket
    close(client_fd);
    return 0;
}