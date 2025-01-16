#include "functions.h"

int main()  {
    fd_set readfds;
    char client_names[MAX_CLIENTS][BUFFER_SIZE];
    int client_fds[MAX_CLIENTS];

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
    if (wgetstr(message_win, name) == 1) {
        refresh();
        wclear(message_win);
        box(message_win, 0, 0);
        wrefresh(message_win);
    }
    name[strlen(name)] = '\0';

    write(client_socket, name, sizeof(name));

    FD_ZERO(&readfds);
    FD_SET(client_socket, &readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    while (1) {
        refresh();
        wclear(message_win);
        box(message_win, 0, 0);
        wrefresh(message_win);
        mvwprintw(message_win, 1, 1, "Write your message");
        mvwprintw(message_win, 2, 1, "> ");
        fd_set tempfds = readfds; 

        if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) < 0) {
            perror("select() failed");
            exit(1);
        }

        if (FD_ISSET(client_socket, &tempfds)) {
            // refresh();
            // clear_chat(chat_win);
            // update_user_win(user_win, client_fds, client_names);
            // print_chat(chat_win);
            
            char buffer[256];
            int bytesRead = recv(client_socket, buffer, sizeof(buffer), MSG_DONTWAIT);
            if (bytesRead > 0) {
                refresh();
                clear_chat(chat_win);
                update_user_win(user_win, client_fds, client_names);
                print_chat(chat_win);
            } else {
                printf("Server disconnected.\n");
                delwin(chat_win);
                delwin(user_win);
                delwin(message_win);
                break;
            }

            // if (signal == update_signal) { 
            //     refresh();
            //     clear_chat(chat_win);
            //     print_chat(chat_win);
            // }
        }

        // Remove newline character
        //message[strlen(message)] = '\0';

        if (FD_ISSET(STDIN_FILENO, &tempfds)) {
            mvwprintw(message_win, 1, 1, "Write your message");
            mvwprintw(message_win, 2, 1, "> ");
            char message[BUFFER_SIZE];
            if (wgetstr(message_win, message) == 0) {
                refresh();
                wclear(message_win);
                box(message_win, 0, 0);
                wrefresh(message_win);
            }

            int bytesWritten = write(client_socket, message, strlen(message) + 1);
            if (bytesWritten < 0) {
                perror("client write error");
                exit(1);
            }
        }
    }

    close(client_socket);
    freeaddrinfo(res);
    return 0;
}