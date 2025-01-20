#include "functions.h"

int client_socket;
int user_socket;

struct addrinfo hints, *res;
// struct addrinfo hints2, *res2;

WINDOW *chat_win;
WINDOW *user_win;
WINDOW *message_win;

static void sighandler(int signo) {
    close(client_socket);
    close(user_socket);
    freeaddrinfo(res);
    // freeaddrinfo(res2);
    printf("Disconnected from server.\n");
    exit(0);
}


void update_user_list() {
    char client_names[MAX_CLIENTS][BUFFER_SIZE];
    memset(client_names, '\0', sizeof(client_names));
    clear_window(user_win);
    int bytes = read(user_socket, client_names, sizeof(client_names));
    if (bytes < 0) {
        perror("EROROEOREOOREOROE AHGAHHHFHFAHHF");
        exit(1);
    }
    update_user_win(user_win, client_names);
}


void update_chat(int chat, char *name) {
    display_message_prompt(message_win);

    char buffer[256];
    int bytes = recv(client_socket, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (bytes > 0) {
        bytes = write(chat, buffer, strlen(buffer));
        if (bytes < 0) {
            perror("write to chat");
            exit(1);
        }
        wprintw(message_win, "%s\n", buffer);
        refresh();
        clear_window(chat_win);
        print_chat(chat_win, name);
    } 
    else {
        printf("Server closed.\n");
        delwin(chat_win);
        delwin(user_win);
        delwin(message_win);
    }
}

void handle_input() {
    display_message_prompt(message_win);
    char message[BUFFER_SIZE];
    if (wgetstr(message_win, message) == 0) {
        display_message_prompt(message_win);
    }
    int bytes = write(client_socket, message, strlen(message) + 1);
    if (bytes < 0) {
        perror("client write error");
        exit(1);
    }
}


int main(int argc, char *argv[])  {
    signal(SIGINT, sighandler);
    fd_set readfds;
    int max_descriptor = 0;

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
        perror("client socket");
        return 1;
    }

    if (connect(client_socket, res->ai_addr, res->ai_addrlen) == -1) {
        perror("client connect");
        close(client_socket);
        return 1;
    }

    // creating socket connection from usernames
    //char *host2 = argv[1];
    //memset(&hints2, 0, sizeof(hints2));
    //hints2.ai_family = AF_INET;
    //hints2.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, USER_PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    user_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (user_socket == -1) {
        perror("user socket");
        return 1;
    }

    if (connect(user_socket, res->ai_addr, res->ai_addrlen) == -1) {
        perror("user connect");
        close(user_socket);
        return 1;
    }


    mvwprintw(chat_win, 1, 1, "Connected to server.");
    wrefresh(chat_win);

    char name[256];
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
        perror("write username");
        exit(1);
    }


    int chat = create_chat(name);

    FD_ZERO(&readfds);
    FD_SET(client_socket, &readfds);
    FD_SET(user_socket, &readfds);
    FD_SET(STDIN_FILENO, &readfds);
    if (user_socket > client_socket) {
        max_descriptor = user_socket;
    }
    else {
        max_descriptor = client_socket;
    }
    
    int debug = 0;
    char connect_message[BUFFER_SIZE] = "joined the chat!\n";
    bytes = write(client_socket, connect_message, strlen(connect_message) + 1);
    if (bytes < 0) {
        perror("client write connected error");
        exit(1);
    }

    while (1) {
        display_message_prompt(message_win);
        fd_set tempfds = readfds; 

        if (select(max_descriptor + 1, &tempfds, NULL, NULL, NULL) < 0) {
            perror("select() failed");
            exit(1);
        }

        for (int i = 0; i <= max_descriptor; i++) {
            if (!FD_ISSET(i, &tempfds)) {
                continue;
            }
            if (i == client_socket) {
                update_chat(chat, name);
            }
            else if (i == user_socket) {
                update_user_list();
            }
            else if (i == STDIN_FILENO) {
                handle_input();
            }
        }
    }

    close(client_socket);
    freeaddrinfo(res);
    return 0;
}
