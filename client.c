#include "functions.h"

int client_socket;
struct addrinfo *res;

WINDOW *chat_win;
WINDOW *user_win;
WINDOW *message_win;

static void sighandler(int signo) {
    close(client_socket);
    freeaddrinfo(res);
    printf("Disconnected from server.\n");
    exit(0);
}


int main(int argc, char *argv[])  {
    signal(SIGINT, sighandler);
    fd_set readfds;

    char client_names[MAX_CLIENTS][BUFFER_SIZE];
    int client_fds[MAX_CLIENTS];

    chat_win = create_chat_win();
    user_win = create_user_win();
    message_win = create_message_win();
    refresh();
    
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

    mvwprintw(chat_win, 1, 1, "Connected to server.");
    wrefresh(chat_win);
    char welcome_message[256];
    char name[256];
    bytes = read(client_socket, welcome_message, sizeof(welcome_message));
    if (bytes < 0) {
        perror("read welcome message");
        exit(1);
    }


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

    bytes = write(client_socket, name, sizeof(name));
    if (bytes < 0) {
        perror("fail to write user name to server");
        exit(1);
    }

    int chat = create_chat(name);

    FD_ZERO(&readfds);
    FD_SET(client_socket, &readfds);
    FD_SET(STDIN_FILENO, &readfds);
    

    // tell everyone new client connected
    char connect_message[BUFFER_SIZE] = "joined the chat!\n";
    bytes = write(client_socket, connect_message, strlen(connect_message) + 1);
    if (bytes < 0) {
        perror("client write connected error");
        exit(1);
    }

    while (1) {
        display_message_prompt(message_win);
        fd_set tempfds = readfds; 

        if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) < 0) {
            perror("select() failed");
            exit(1);
        }

        if (FD_ISSET(client_socket, &tempfds)) {
            display_message_prompt(message_win);
            
            char buffer[256];
            bytes = recv(client_socket, buffer, sizeof(buffer), MSG_DONTWAIT);
            if (bytes > 0) {
                bytes = write(chat, buffer, strlen(buffer));
                wprintw(message_win, "%s\n", buffer);
                if (bytes < 0) {
                    perror("line 108 error");
                    exit(1);
                }
                refresh();
                clear_window(chat_win);
                //update_user_win(user_win, client_fds, client_names);
                print_chat(chat_win, name);
            } else {
                printf("Server disconnected.\n");
                delwin(chat_win);
                delwin(user_win);
                delwin(message_win);
            }

        }

        if (FD_ISSET(STDIN_FILENO, &tempfds)) {
            display_message_prompt(message_win);
            char message[BUFFER_SIZE];
            if (wgetstr(message_win, message) == 0) {
                display_message_prompt(message_win);
            }

            bytes = write(client_socket, message, strlen(message) + 1);
            if (bytes < 0) {
                perror("client write error");
                exit(1);
            }
        }
    }

    close(client_socket);
    freeaddrinfo(res);
    return 0;
}