#include "functions.h"


int server_setup() {
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo
  hints = calloc(1,sizeof(struct addrinfo));

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

int handle_new_connection(int server_socket, int client_fds[], char client_names[][BUFFER_SIZE]) {
    int client_socket = server_connect(server_socket);

    char welcome_message[256] = "Welcome to the chat!";
    if (write(client_socket, welcome_message, sizeof(welcome_message)) < 0) {
        perror("welcome message");
        exit(1);
    }

    char name[BUFFER_SIZE];
    if (read(client_socket, name, BUFFER_SIZE) < 0) {
        perror("read name");
        exit(1);
    }

    // add new client to list of fd
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] == 0) {
            client_fds[i] = client_socket;
            strcpy(client_names[i], name);
            printf("Client [%s] connected\n", client_names[i]);
            break;
        }
    }
    return client_socket;
}


void read_from_client(char message[], int fd, int client_fds[], char client_names[][BUFFER_SIZE], fd_set *read_fds) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes;

    bytes = read(fd, buffer, sizeof(buffer));
    if (bytes < 0) {
        // client disconnects
        return;
    }
    else if (bytes == 0) {
        // done reading, remove socket from list of available file descriptors to read from
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] == fd) {
                client_fds[i] = 0;
                printf("%s disconnected!\n", client_names[i]);
                strcpy(client_names[i], "");
                break;
            }
        }
        FD_CLR(fd, read_fds);
        return;
    }

    //printf("received from client '%s' (read %d bytes)\n", buffer, bytesRead);
    //printf("before writing to chat.txt\n");
    // send message to chat
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] == fd) {
            // append name
            sprintf(message, "[%s] %s\n", client_names[i], buffer);
            break;
        }
    }
}


void write_to_clients(char message[], int client_fds[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_fds[i] > 0) {
            int bytes = write(client_fds[i], message, strlen(message) + 1);
            if (bytes < 0) {
                perror("write to client error");
                exit(1);
            }
        }
    }
}


int create_chat(char username[]) {
    char filename[256] = "chat_";
    strcat(filename, username);
    strcat(filename, ".txt");
    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
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
void print_chat(char username[]) {
    char filename[256] = "chat_";
    strcat(filename, username);
    strcat(filename, ".txt");
    FILE *file = fopen(filename, "r");
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
