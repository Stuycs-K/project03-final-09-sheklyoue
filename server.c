#include "functions.h" 

int main() {
    int max_descriptor = 0;
    int server_socket = server_setup(); 

    fd_set read_fds;
    
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);
    max_descriptor = server_socket; 
    
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
                printf("new client connected\n");
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
                    FD_CLR(i, &read_fds); 
                    printf("a client disconnected\n");
                }
                else {
                    printf("received from client '%s' (read %d bytes)\n", buffer, bytesRead);

                    // do something (send message to other clients)

                }


            }
        }
    }
}