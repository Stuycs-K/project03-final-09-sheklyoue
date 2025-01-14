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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define BUFFER_SIZE 1000
#define MAX_CLIENTS 25
#define WKP "lawric"
#define CHAT "chat.txt"
#define update_signal "update_SYN"
#define PORT "9998"

struct user {
  char username[100];
};

int server_setup();
int server_connect(int listen_socket);
int handle_new_connection(int server_socket, int client_fds[], char client_names[][BUFFER_SIZE]);
void read_from_clients(int fd, int client_fds, char client_names[][BUFFER_SIZE]);
void view_users();
int create_chat();
void send_public_message();
void print_chat();
void clear_chat();

/*
functions to add later 
int send_private_message();
void connect();
*/

#endif
