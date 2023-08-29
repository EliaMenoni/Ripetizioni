#ifndef COM_H_
#define COM_H_

#define MSG_QUERY 'Q'
#define MSG_LOAN 'L'
#define MSG_RECORD 'R'
#define MSG_NO 'N'
#define MSG_ERROR 'E'

struct Packet {
    char type;              // Tipo richiesta
    unsigned int length;    // Lunghezza richiesta
    char data[100];         // Dati richiesta
};
typedef struct Packet Packet;

#endif