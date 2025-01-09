#include "functions.h"


void server_setup() {

}

//Create the user with an username for tracking
void create_user() {

}

//Prints out a list of all the current users in the chat
void view_users() {

}

//Prints the chat on the terminal
void print_chat(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(1);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
}

//Clears all text on the terminal
void clear_terminal() {
    char *args[] = {"clear", NULL};
    if (execvp("clear", args) == -1) {
        perror("execvp clear failed");
        exit(1);
    }
}

void send_public_message() {
    
}

//Creates the chat and allows for the chatting 
void create_chat() {

}