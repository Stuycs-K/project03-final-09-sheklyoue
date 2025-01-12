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

  printf("Connected to server.\n");
  char welcome_message[256];
  char name[256];
  read(client_socket, welcome_message, sizeof(welcome_message));
  printf("%s\n", welcome_message);
  printf("Please input your name: ");
  fgets(name, sizeof(name), stdin);
  name[strlen(name) - 1] = '\0';
  write(client_socket, name, sizeof(name));

  while (1) {
        printf("Write your message\n");
        printf("> ");
        char message[BUFFER_SIZE];
        fgets(message, sizeof(message), stdin);

        // Remove newline character
        message[strlen(message) - 1] = '\0';

        printf("Sending message '%s'\n", message);
        int bytesWritten = write(client_socket, message, strlen(message) + 1);
        if (bytesWritten < 0) {
          perror("client write error");
          exit(1);
        }

        char buffer[BUFFER_SIZE];
        int bytesRead = recv(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
        if (bytesRead == 0) {
            printf("Server disconnected.\n");
            break;
        } 
        else if (bytesRead > 0) {
          printf("received message: '%s'\n", buffer);
        }
  }

  close(client_socket);
  freeaddrinfo(res);
  return 0;
}