#include "functions.h"

int client_socket;
int user_socket;

struct addrinfo hints, *res;
struct addrinfo hints2, *res2;

WINDOW *chat_win;
WINDOW *user_win;
WINDOW *message_win;

static void sighandler(int signo) {
    close(client_socket);
    freeaddrinfo(res);
    freeaddrinfo(res2);
    printf("Disconnected from server.\n");
    exit(0);
}


int main(int argc, char *argv[])  {
    signal(SIGINT, sighandler);
    fd_set readfds;

    char client_names[MAX_CLIENTS][BUFFER_SIZE];

    chat_win = create_chat_win();
    user_win = create_user_win();
    message_win = create_message_win();
    refresh();

    int bytes;

    if (argc != 2) {
        fprintf(stderr,"usage: ./client hostname\n");
        exit(1);
    }

    // creating socket connection for messages 
    char *host = argv[1];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, CHAT_PORT, &hints, &res) != 0) {
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

    // creating socket connection from usernames
    char *host2 = argv[1];
    memset(&hints2, 0, sizeof(hints2));
    hints2.ai_family = AF_INET;
    hints2.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host2, USER_PORT, &hints2, &res2) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    user_socket = socket(res2->ai_family, res2->ai_socktype, res2->ai_protocol);
    if (client_socket == -1) {
        perror("socket");
        return 1;
    }

    if (connect(user_socket, res2->ai_addr, res2->ai_addrlen) == -1) {
        perror("connect");
        close(user_socket);
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

    int chat = create_chat(name);

    FD_ZERO(&readfds);
    FD_SET(client_socket, &readfds);
    FD_SET(user_socket, &readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    while (1) {
        display_message_prompt(message_win);
        fd_set tempfds = readfds; 

        if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) < 0) {
            perror("select() failed");
            exit(1);
        }

        if (FD_ISSET(user_socket, &tempfds)) {
            read(user_socket, client_names, sizeof(client_names));
            update_user_win(user_win, client_names);
        }

        if (FD_ISSET(client_socket, &tempfds)) {
            display_message_prompt(message_win);
        
            char buffer[256];
            int bytesRead = recv(client_socket, buffer, sizeof(buffer), MSG_DONTWAIT);
            if (bytesRead > 0) {
                int bytes = write(chat, buffer, strlen(buffer));
                wprintw(message_win, "%s\n", buffer);
                if (bytes < 0) {
                    perror("line 108 error");
                    exit(1);
                }
                refresh();
                clear_window(chat_win);
                //update_user_win(user_win, client_names);
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
