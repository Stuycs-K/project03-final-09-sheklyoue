#include "functions.h"

int to_server;
int from_server;


void sighandler(int signo) {
  close(to_server);
  close(from_server);
  printf("\nclient disconnected\n");
  exit(0);
}


int main() {
  signal(SIGINT, sighandler);

  from_server = client_handshake(&to_server);

  while (1) {
    // prompt user for message to send
    char message[BUFFER_SIZE];
    printf("input message: ");
    fgets(message, BUFFER_SIZE - 1, stdin);
    // get rid of trailing '\n'
    message[strlen(message) - 1] = '\0';
    // send message to server
    printf("sending message...\n");
    write(to_server, message, strlen(message) + 1);

    // read any messages that were sent to the server
    char buffer[BUFFER_SIZE];
    int bytesRead = read(from_server, buffer, BUFFER_SIZE);
    if (bytesRead < 0) {
      perror("client couldn't receive message from server");
      exit(1);
    }
    printf("client received: %s\n", buffer);
  }
  close(to_server);
  close(from_server);
}
