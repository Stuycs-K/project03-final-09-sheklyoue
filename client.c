#include "functions.h"


int main()  {
  struct addrinfo hints, *res;
  char *host = "127.0.0.1";
  char *port = "9998";

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(host, port, &hints, &res) != 0) {
      perror("getaddrinfo");
      return 1;
  }

  int client_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (client_socket == -1) {
      perror("socket");
      return 1;
  }

  if (connect(client_socket, res->ai_addr, res->ai_addrlen) == -1) {
      perror("connect");
      close(client_socket);
      return 1;
  }

  printf("Connected to server. Type messages below:\n");
  while (1) {
        printf("> ");
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline character
        buffer[strlen(buffer) - 1] = '\0';

        int bytes = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes == 0) {
            printf("Server disconnected.\n");
            break;
        } else if (bytes < 0) {
            perror("Read error");
            break;
        }
            
        printf("Sending message '%s'\n", buffer);
        write(client_socket, buffer, strlen(buffer) + 1);
  }

  close(client_socket);
  freeaddrinfo(res);
  return 0;
}