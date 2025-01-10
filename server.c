#include "functions.h"

void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("\nServer shutting down. Removing WKP.\n");
        remove(WKP);
        exit(0);
    }
}

int main() {
    int to_client;
    int from_client;
    signal(SIGINT, sighandler);
    while (1) {
        printf("Waiting for a client ...\n");
        from_client = server_handshake(&to_client);
        printf("Connected to a client!\n");

        char buffer[BUFFER_SIZE];
        while (1) {
            int bytesRead = read(from_client, buffer, sizeof(buffer));
            // client disconnects
            if (bytesRead == 0) {
                printf("a client disconnected\n");
                break;
            }
            if (bytesRead < 0) {
                perror("server fail to read message from client");
                exit(1);
            }
            printf("bytes read: %d\n", bytesRead);
            printf("Server received message : '%s'\n", buffer);
            
            // send to client
            int bytesWritten = write(to_client, buffer, strlen(buffer) + 1);
            if (bytesWritten < 0) {
                perror("server fail to send message to client");
                exit(1);
            }
        }
        close(from_client);
        close(to_client);
    }
}
