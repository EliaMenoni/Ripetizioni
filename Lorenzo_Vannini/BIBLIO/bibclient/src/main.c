#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "../../bibserver/lib/libro/libro.h"
#include "../../bibserver/lib/comprot/comprot.h"

#define PORT 8002

int main(int argc, char const *argv[])
{
    Packet request;
    request.type = MSG_LOAN;
    strcpy(request.data, "titolo:Il segreto del successo;");
    request.length = 1;

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

    send(client_fd, &request, sizeof(Packet), 0);

    Packet ricevuto;
    do
    {
        valread = read(client_fd, &ricevuto, sizeof(Packet));
        if (ricevuto.type != MSG_NO)
            // stampa_libro(&ricevuto);
            printf("Libro: %s\n", ricevuto.data);
    } while (ricevuto.type != MSG_NO);

    // closing the connected socket
    close(client_fd);
    return 0;
}