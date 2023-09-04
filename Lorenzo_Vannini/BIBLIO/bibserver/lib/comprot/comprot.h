#ifndef COM_H_
#define COM_H_

#include <arpa/inet.h>
#include <netinet/in.h>

#define MSG_QUERY 'Q'
#define MSG_LOAN 'L'
#define MSG_RECORD 'R'
#define MSG_NO 'N'
#define MSG_ERROR 'E'

struct Packet {             // Richiesta e risposta che il client manda al server e viceversa
    char type;              // Tipo richiesta
    unsigned int length;    // Lunghezza richiesta
    char data[2048];         // Dati richiesta
};
typedef struct Packet Packet;

int create_socket_file();   

void setup_socket_data(char *IP, int port, struct sockaddr_in *addr);

void socket_bind(int socket, struct sockaddr_in *addr);

void server_listen(int socket);

int server_accept(int socket, struct sockaddr_in *addr);

#endif