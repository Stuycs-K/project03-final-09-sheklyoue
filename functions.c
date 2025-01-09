#include "functions.h"


int server_setup() {
    //printf("Setting server up\n");
    //printf("1. Server Making Pipe\n");
    int from_client;
    if (mkfifo(WKP, 0666) == -1) {
        perror("Named Pipe Error");
        exit(1);
    }
    
    //printf("2. Server Opening WKP\n");
    from_client = open(WKP, O_RDONLY);
    if (from_client == -1) {
        perror("wkp open error");
        exit(1);
    }

    //printf("4. Server removing WKP\n");
    remove(WKP);
    //printf("Setting Server up done\n");
    //printf("from_client = %d\n", from_client);
    return from_client;
}

int server_handshake(int *to_client) {
    int from_client = server_setup();
    
    char client_pipe[BUFFER_SIZE];
    read(from_client, client_pipe, sizeof(client_pipe));
    //printf("5. Server received SYN: %s\n", client_pipe);

    //printf("6. Server opening Private Pipe\n");
    *(to_client) = open(client_pipe, O_WRONLY);
    if (*(to_client) == -1) {
        perror("open client pipe");
        exit(1);
    }

    char syn_ack[] = "SYN_ACK";
    //printf("7. Server sending SYN_ACK\n");
    write(*(to_client), syn_ack, sizeof(syn_ack));

    char ack[BUFFER_SIZE];
    read(from_client, ack, sizeof(ack));
    //printf("9. Server received ACK: %s\n", ack);

    //printf("Handshake complete\n");
    //printf("from_client = %d\n", from_client);
    return from_client;
}

int client_handshake(int *to_server) {
    int from_server;
    //printf("3. Client creating Private Pipe\n");
    char private_pipe[BUFFER_SIZE];
    snprintf(private_pipe, sizeof(private_pipe), "pipe_%d", getpid());
    if (mkfifo(private_pipe, 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    //printf("3. Client opening WKP\n");
    *(to_server) = open(WKP, O_WRONLY);
    if (*(to_server) == -1) {
        perror("open WKP");
        remove(private_pipe);
        exit(1);
    }
    int client_wkp = *(to_server);

    //printf("3. Client writing Private Pipe name to WKP\n");
    write(client_wkp, private_pipe, sizeof(private_pipe));

    //printf("3. Client opening Private Pipe (blocking)\n");
    from_server = open(private_pipe, O_RDONLY);
    if (from_server == -1) {
        perror("open private pipe");
        remove(private_pipe);
        exit(1);
    }
    remove(private_pipe);

    char syn_ack[BUFFER_SIZE];
    read(from_server, syn_ack, sizeof(syn_ack));
    //printf("8. Client received SYN_ACK: %s\n", syn_ack);

    //printf("8. Client sending ACK on WKP\n");
    char ack[] = "ACK";
    write(client_wkp, ack, sizeof(ack));
    //printf("to_server = %d\n", client_wkp);
    return from_server;
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