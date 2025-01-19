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
#define update_signal 2222
#define CHAT_PORT "9998"
#define USER_PORT "9997"

extern char client_names[MAX_CLIENTS][BUFFER_SIZE];
extern int client_fds[MAX_CLIENTS];

int server_setup(char* port_id);
int server_connect(int listen_socket);
void update_user_win(WINDOW *win, char client_names[][BUFFER_SIZE]);
void clear_window(WINDOW *win);
void display_message_prompt(WINDOW *message_win);

WINDOW *create_chat_win();
WINDOW *create_user_win();
WINDOW *create_message_win();


int handle_new_connection(int server_socket, int user_socket, int client_fds[], char client_names[][BUFFER_SIZE], int user_fds[]);
void read_from_client(char message[], int fd, int client_fds[], char client_names[][BUFFER_SIZE], fd_set *read_fds);
void write_to_clients(char message[], int client_fds[]);
void view_users();
int create_chat(char username[]);
void print_chat(WINDOW *win, char username[]);

#endif