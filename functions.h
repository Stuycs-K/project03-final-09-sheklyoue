#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <signal.h> 
#include <time.h> 
#include <ncurses.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define BUFFER_SIZE 1000
#define MAX_CLIENTS 25
#define WKP "lawric"
#define CHAT "chat.txt"
#define update_signal "update_SYN"
extern char client_names[MAX_CLIENTS][BUFFER_SIZE];
extern int client_fds[MAX_CLIENTS];

int server_setup();
int server_connect(int listen_socket);
void update_user_win(WINDOW *win);
int create_chat_log();
void send_public_message();
void print_chat(WINDOW *win);
void clear_chat(WINDOW *win);

WINDOW *create_chat_win();
WINDOW *create_user_win();
WINDOW *create_message_win();

/*
functions to add later 
int send_private_message();
void connect();
*/

#endif
