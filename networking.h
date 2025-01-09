#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifndef NETWORKING_H
#define NETWORKING_H

int server_setup();
void view_users();
int create_user();
void create_chat();
int send_public_message();
int send_private_message();g
void connect();

//functions to add later

#endif
