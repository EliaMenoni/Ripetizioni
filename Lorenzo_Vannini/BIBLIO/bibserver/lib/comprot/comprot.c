#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include "../factory/factory.h"

int create_socket_file() {
  int server_fd;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  return server_fd;
}

void setup_socket_data(char *IP, int port, struct sockaddr_in *addr) {
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = inet_addr(IP);
  addr->sin_port = htons(port);
}

void socket_bind(int socket, struct sockaddr_in *addr) {
  if (bind(socket, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
}

void server_listen(int socket) {
  if (listen(socket, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

int server_accept(int socket, struct sockaddr_in *addr) {
  int new_socket;
  int addrlen = sizeof(*addr);
  if ((new_socket = accept(socket, (struct sockaddr *)addr, (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return new_socket;
}
