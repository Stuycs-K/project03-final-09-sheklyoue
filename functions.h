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

struct user {
  char username[100];
};

int server_setup();
int server_connect(int listen_socket);
void view_users();
int create_chat();
void send_public_message();
void print_chat();
void clear_terminal();

/*
functions to add later 
int send_private_message();
void connect();
*/

#endif
