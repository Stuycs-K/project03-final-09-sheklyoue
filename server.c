#include "functions.h" 

char client_names[MAX_CLIENTS][BUFFER_SIZE];
int client_fds[MAX_CLIENTS];

int main() {
    
    for (int c = 0; c < MAX_CLIENTS; c++) {
        client_fds[c] = 0;
        strcpy(client_names[c], "");
    }
    int max_descriptor = 0;
    int server_socket = server_setup(); 

    fd_set read_fds;
    
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);
    max_descriptor = server_socket; 


    // create chat file
    int chat = create_chat_log(); 
    
    while (1) {
        // create a copy since select modifies the set
        fd_set copy = read_fds;
        // waits until any number of fds are available
        int available = select(max_descriptor + 1, &copy, NULL, NULL, NULL);
        if (available < 0) {
            perror("select error");
            exit(1);
        }
        // check each fd to see if it has data to read from
        for (int i = 0; i <= max_descriptor; i++) {
            if (!FD_ISSET(i, &copy)) {
                continue;
            }
            if (i == server_socket) {
                // accept new connection 
                int client_socket = server_connect(server_socket);  
                //printf("new client connected\n");

                char welcome_message[256] = "Welcome to the chat!";
                write(client_socket, welcome_message, sizeof(welcome_message));

                char name[BUFFER_SIZE];
                read(client_socket, name, BUFFER_SIZE);

                // add new client to list of fd
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_fds[i] == 0) {
                        client_fds[i] = client_socket;
                        //char temp_name[BUFFER_SIZE] = "TEMP";
                        strcpy(client_names[i], name);
                        printf("Client [%s] connected\n", client_names[i]);
                        // printf("Adding to list of sockets at index %d\n", i);
                        break;
                    }
                }
                // add socket to fdset
                FD_SET(client_socket, &read_fds);
                // update max_descriptor 
                if (client_socket > max_descriptor) {
                    max_descriptor = client_socket; 
                }
            }
            else {
                // receive message from a client 
                char buffer[BUFFER_SIZE]; 
                memset(buffer, 0, BUFFER_SIZE);

                int bytesRead = read(i, buffer, sizeof(buffer));
                if (bytesRead < 0) {
                    perror("server read error");
                    exit(1);
                }

                if (bytesRead == 0) {
                    // done reading, remove socket from list of available file descriptors to read from
                    for(int c = 0; c < MAX_CLIENTS; c++) {
                        if (client_fds[c] == i) {
                            client_fds[c] = 0;
                            printf("%s disconnected!\n", client_names[c]);
                            strcpy(client_names[c], ""); 
                            break;
                        }
                    }
                    FD_CLR(i, &read_fds); 
                }
                else {
                    //printf("received from client '%s' (read %d bytes)\n", buffer, bytesRead);
                    //printf("before writing to chat.txt\n");
                    // send message to chat
                    char message[BUFFER_SIZE];
                    for (int c = 0; c < MAX_CLIENTS; c++) {
                        if (client_fds[c] == i) {
                            // append name
                            sprintf(message, "[%s] %s\n", client_names[c], buffer);
                            break;
                        }
                    }

                    int bytesWritten = write(chat, message, strlen(message));
                    if (bytesWritten < 0) {
                        perror("server write error");
                    }

                    //printf("sending signal");
                    for (int c = 0; c < MAX_CLIENTS; c++) {
                        if (client_fds[c] > 0) {
                            write(client_fds[c], update_signal, sizeof(update_signal));
                        }
                    }
                }

            }
        }
    }
    endwin();
}