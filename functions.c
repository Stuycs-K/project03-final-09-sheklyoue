#include "functions.h"


int server_setup() {
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo
  hints = calloc(1,sizeof(struct addrinfo));
  char* PORT = "9998";

  hints->ai_family = AF_INET; // domain of socket (tpye of address) 
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, PORT, hints, &results);  //NULL is localhost or 127.0.0.1

  //create listen socket
  int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  //this code allows the port to be freed after program exit.  (otherwise wait a few minutes)
  int yes = 1;
  if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      printf("sockopt  error\n");
      printf("%s\n",strerror(errno));
      exit(-1);
  }

  // bind listening socket to address 
  int err = bind(listen_socket, results->ai_addr, results->ai_addrlen);
  if(err == -1){
      printf("Error binding: %s",strerror(errno));
      exit(1);
  }
  // listen socket  
  listen(listen_socket, MAX_CLIENTS);//3 clients can wait to be processed
  printf("Listening on port %s\n",PORT);

  return listen_socket;
}


int server_connect(int listen_socket) {
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);
    
    int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
    return client_socket;
}

//Creates the chat and allows for the chatting 
int create_chat() {
    int fd = open(CHAT, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open chat file error");
        exit(1);
    }
    return fd;
}


//Prints out a list of all the current users in the chat
void view_users() {

}


//Prints the chat on the terminal
void print_chat() {
    FILE *file = fopen(CHAT, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
}


//Clears all text on the terminal
void clear_chat() {
    system("clear");
}


void send_public_message() {
    
}



