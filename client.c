#include "functions.h"


int main()  {
  WINDOW *chat_win = create_chat_win();
  WINDOW *user_win = create_user_win();
  WINDOW *message_win = create_message_win();
  refresh();

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

  mvwprintw(chat_win, 1, 1, "Connected to server.");
  wrefresh(chat_win);
  char welcome_message[256];
  char name[256];
  read(client_socket, welcome_message, sizeof(welcome_message));

  wprintw(chat_win, "%s\n", welcome_message);
  mvwprintw(message_win, 1, 1, "Please input your name: ");
  wrefresh(message_win);
  wgetstr(message_win, name);
  name[strlen(name) - 1] = '\0';

  write(client_socket, name, sizeof(name));

  while (1) {
        clear_chat(chat_win);
        update_user_win(user_win);
        wrefresh(chat_win);
        //print_chat(win);
        
        char buffer[BUFFER_SIZE];
        int bytesRead = recv(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);
        if (bytesRead == 0) {
            printf("Server disconnected.\n");
            delwin(chat_win);
            delwin(user_win);
            delwin(message_win);
            break;
        } 
        else if (bytesRead > 0) {
          //printf("received message: '%s'\n", buffer);
        }

        //mvwprintw(win, 49, 1, "Write your message\n");
        //mvwprintw(win, 50, 1, "> ");
        char message[BUFFER_SIZE];
        //wgetstr(win, message);

        // Remove newline character
        message[strlen(message) - 1] = '\0';

        //printf("Sending message '%s'\n", message);
        int bytesWritten = write(client_socket, message, strlen(message) + 1);
        if (bytesWritten < 0) {
          perror("client write error");
          exit(1);
        }

  }

  close(client_socket);
  freeaddrinfo(res);
  return 0;
}