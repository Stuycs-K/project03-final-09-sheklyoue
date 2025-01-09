#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct user {
  char username[100];
};

void server_setup();
void view_users();
void create_user();
void create_chat();
void send_public_message();
void print_chat();
void clear_terminal();

/*
functions to add later 
int send_private_message();
void connect();
*/

#endif