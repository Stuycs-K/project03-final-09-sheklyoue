#include "functions.h"

int client_socket;
struct addrinfo *res;

static void sighandler(int signo) {
  close(client_socket);
  freeaddrinfo(res);
  printf("\nDisconnected from server.\n");
  exit(0);
}


int main(int argc, char *argv[])  {
  signal(SIGINT, sighandler);
  struct addrinfo hints, *res;
  int bytes;

  if (argc != 2) {
    fprintf(stderr,"usage: ./client hostname\n");
    exit(1);
  }
  char *host = argv[1];
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(host, PORT, &hints, &res) != 0) {
      perror("getaddrinfo");
      return 1;
  }

  client_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (client_socket == -1) {
      perror("socket");
      return 1;
  }

  if (connect(client_socket, res->ai_addr, res->ai_addrlen) == -1) {
      perror("connect");
      close(client_socket);
      return 1;
  }

  // create chat file 
  int chat = create_chat(); 

  
  printf("Connected to server.\n");
  char welcome_message[256];
  char name[256];
  bytes = read(client_socket, welcome_message, sizeof(welcome_message));
  if (bytes < 0) {
    perror("failed to read welcome message");
    exit(1);
  }

  printf("%s\n", welcome_message);
  printf("Please input your name: ");
  fgets(name, sizeof(name), stdin);
  name[strlen(name) - 1] = '\0';
  bytes = write(client_socket, name, sizeof(name));
  if (bytes < 0) {
    perror("failed to send name to server");
    exit(1);
  }



  while (1) {
        clear_chat();
        
        char buffer[BUFFER_SIZE];
        bytes = recv(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
        if (bytes == 0) {
            printf("Server closed.\n");
            break;
        } 
        else if (bytes > 0) {
          bytes = write(chat, buffer, strlen(buffer) + 1);
          if (bytes < 0) {
            perror("server write chat");
            exit(1);
          }
          print_chat();
        }

        printf("Write your message\n");
        printf("> ");
        char message[BUFFER_SIZE];
        fgets(message, sizeof(message), stdin);

        // Remove newline character
        message[strlen(message) - 1] = '\0';

        //printf("Sending message '%s'\n", message);
        bytes = write(client_socket, message, strlen(message) + 1);
        if (bytes < 0) {
          perror("client write error");
          exit(1);
        }

  }

  close(client_socket);
  freeaddrinfo(res);
  return 0;
}