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
int create_chat_log() {
    int fd = open(CHAT, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
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
void update_user_win(WINDOW *win) {
    int h = 1;
    for (int c = 0; c < MAX_CLIENTS; c++) {
        if (client_fds[c] > 0) {
            mvwprintw(win, h, 1, "%s", client_names[c]);
            h++;
        }
    }
}


//Prints the chat on the terminal
void print_chat(WINDOW *win) {
    FILE *file = fopen(CHAT, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        wprintw(win,"%s", buffer);
    }
    wprintw(win, "\n");
    wrefresh(win);
    fclose(file);
}


//Clears all text on the terminal
void clear_chat(WINDOW *win) {
    wclear(win);
    wrefresh(win);
}


void send_public_message() {
    
}



