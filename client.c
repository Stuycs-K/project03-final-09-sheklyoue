#include "functions.h"


int main() {
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  printf("Sending Message\n");
  char message[] = "HELLO!";
  write(to_server, message, sizeof(message));

  char ack[BUFFER_SIZE];
  while(read(from_server, ack, sizeof(ack))) {
    sleep(1);
    printf("Client message : %s\n", ack);
    write(to_server, ack, sizeof(ack));
  }
}
