#include "functions.h"

int server_setup(char* port_id) {
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo
  hints = calloc(1,sizeof(struct addrinfo));

  hints->ai_family = AF_INET; // domain of socket (tpye of address)
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, port_id, hints, &results);  //NULL is localhost or 127.0.0.1

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
  printf("Listening on port %s\n",port_id);

  return listen_socket;
}


int server_connect(int listen_socket) {
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
    return client_socket;
}

int handle_new_connection(int server_socket, int user_socket, int client_fds[], char client_names[][BUFFER_SIZE], int user_fds[]) {
    int client_socket = server_connect(server_socket);
    int new_user_socket = server_connect(user_socket);

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
            user_fds[i] = new_user_socket;
            client_fds[i] = client_socket;
            strcpy(client_names[i], name);
            // server side (for debug) 
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
                // server side print message (for debugging)
                printf("%s disconnected!\n", client_names[i]);

                // send disconnect message to chat
                sprintf(message, "\n%s left the chat :(\n\n", client_names[i]);
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

WINDOW *create_chat_win() {
    initscr();
    cbreak();
    echo();
    keypad(stdscr, TRUE);

    int chat_h = 25, chat_w = 70, chat_y = 5, chat_x = 10;
    WINDOW *win = newwin(chat_h, chat_w, chat_y, chat_x);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    return win;
}

WINDOW *create_user_win() {
    int user_h = 25, user_w = 20, user_y = 5, user_x = 80;
    WINDOW *user_win = newwin(user_h, user_w, user_y, user_x);
    refresh();
    box(user_win, 0, 0);
    wrefresh(user_win);
    return user_win;
}


WINDOW *create_message_win() {
    int message_h = 5, message_w = 70, message_y = 30, message_x = 10;
    WINDOW *message_win = newwin(message_h, message_w, message_y, message_x);
    refresh();
    box(message_win, 0, 0);
    wrefresh(message_win);
    return message_win; 
}

//Prints out a list of all the current users in the chat
void update_user_win(WINDOW *win, char client_names[][BUFFER_SIZE]) {
    int h = 2;
    mvwprintw(win, 1, 1, "USER LIST");
    for (int c = 0; c < MAX_CLIENTS; c++) {
        if (strlen(client_names[c]) > 0 ) {
            refresh();
            mvwprintw(win, h, 1, "%s", client_names[c]);
            h++;
            wrefresh(win);
        }
    }
}


//Prints the chat on the terminal
void print_chat(WINDOW *win, char username[]) {
    char filename[256] = "chat_";
    strcat(filename, username);
    strcat(filename, ".txt");
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    int h = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strlen(buffer)-1] = 0; 
        if (h >= 23) {
            clear_window(win);
            h = 1;
            mvwprintw(win,h, 1, "%s", buffer);
        } else {
            h++;
            mvwprintw(win,h, 1, "%s", buffer);
        }
    }
    wrefresh(win);
    fclose(file);
}


//Clears all text on the terminal
void clear_window(WINDOW *win) {
    wclear(win);
    box(win, 0, 0);
    wrefresh(win);
}

void display_message_prompt(WINDOW *message_win) {
    refresh();
    wclear(message_win);
    box(message_win, 0, 0);
    wrefresh(message_win);
    mvwprintw(message_win, 1, 1, "Write your message");
    mvwprintw(message_win, 2, 1, "> ");
    wrefresh(message_win);
}