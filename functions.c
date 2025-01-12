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
  int listen_socket(results->ai_family, results->ai_socktype, results->ai_protocol);
 
  // bind listening socket to address 
  int err = bind(listen_socket, results->ai_addr, results->ai_addrlen);
  if(err == -1){
      printf("Error binding: %s",strerror(errno));
      exit(1);
  }
  // listen socket  
  listen(listen_socket, 3);//3 clients can wait to be processed
  printf("Listening on port %s\n",PORT);
  return listen_socket; 
}




//Create the user with an username for tracking
void create_user() {

}


//Prints out a list of all the current users in the chat
void view_users() {

}


//Prints the chat on the terminal
void print_chat(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(1);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
}


//Clears all text on the terminal
void clear_terminal() {
    char *args[] = {"clear", NULL};
    if (execvp("clear", args) == -1) {
        perror("execvp clear failed");
        exit(1);
    }
}


void send_public_message() {
    
}


//Creates the chat and allows for the chatting 
void create_chat() {

}