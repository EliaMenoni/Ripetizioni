#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../bibserver/lib/comprot/comprot.h"
#include "../../bibserver/lib/libro/libro.h"

void generate_packet(int argc, char const *argv[], Packet *gen) {
  strcpy(gen->data, "");
  gen->type = MSG_QUERY;
  int len = 0;
  for (int i = 1; i < argc; i += 2) {
    if (strcmp(argv[i], "-autore") == 0) {
      strcat(gen->data, "autore:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-titolo") == 0) {
      strcat(gen->data, "titolo:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-editore") == 0) {
      strcat(gen->data, "editore:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-anno") == 0) {
      strcat(gen->data, "anno:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-nota") == 0) {
      strcat(gen->data, "nota:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-collocazione") == 0) {
      strcat(gen->data, "collocazione:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-luogo_pubblicazione") == 0) {
      strcat(gen->data, "luogo_pubblicazione:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-descrizione_fisica") == 0) {
      strcat(gen->data, "descrizione_fisica:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-prestito") == 0) {
      strcat(gen->data, "prestito:");
      strcat(gen->data, argv[i + 1]);
    } else if (strcmp(argv[i], "-p") == 0) {
      gen->type = MSG_LOAN;
      i--;
    }
    len++;
    if (i + 2 < argc) strcat(gen->data, ";");
  }
  gen->length = len;
}

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    printf("Format:\n");
    printf("\t-titolo <titolo>\n");
    printf("\t-editore <editore>\n");
    printf("\t-anno <anno>\n");
    printf("\t-nota <nota>\n");
    printf("\t-collocazione <collocazione>\n");
    printf("\t-luogo_pubblicazione <luogo_pubblicazione>\n");
    printf("\t-descrizione_fisica <descrizione_fisica>\n");
    printf("\t-prestito <prestito>\n");
    printf("\t-p (per noleggio)\n");

    return 0;
  }

  Packet request;
  generate_packet(argc, argv, &request);
  printf("%s\n", request.data);
  int status, valread, client_fd;
  struct sockaddr_in serv_addr;

  FILE *servers_data;
  servers_data = fopen("/workspaces/Ripetizioni/Lorenzo_Vannini/BIBLIO/bibconf.txt", "r");
  if (servers_data == NULL) exit(1);
  char *riga = malloc(250);
  char *nome, *porta, *save;

  while (fgets(riga, 1000, servers_data)) {
    nome = strtok_r(riga, " ", &save);
    porta = strtok_r(NULL, "\n", &save);

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\n Socket creation error \n");
      return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(porta));

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0) {
      printf("\nConnection Failed with %s\n", nome);
      close(client_fd);
      continue;
    }

    send(client_fd, &request, sizeof(Packet), 0);

    Packet ricevuto;
    do {
      valread = read(client_fd, &ricevuto, sizeof(Packet));
      if (ricevuto.type != MSG_NO)
        // stampa_libro(&ricevuto);
        printf("%s\n", ricevuto.data);
    } while (ricevuto.type != MSG_NO);
    // closing the connected socket
    close(client_fd);
  }
  fclose(servers_data);
  free(riga);
  return 0;
}