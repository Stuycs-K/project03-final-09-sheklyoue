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
        int available = select(max_descriptor, &copy, NULL, NULL, NULL);
        if (available < 0) {
            perror("select error");
            exit(1);
        }

        // check each fd to see if it has data to read from
        for (int i = 0; i < max_descriptor; i++) {
            if (!FD_ISSET(i, &read_fds)) {
                continue;
            }
            if (i == server_socket) {
                // accept new connection 
                int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
                FD_SET(client_socket, &read_fds);
                if (client_socket > max_descriptor) {
                    max_descriptor = to_client; 
                }
            }
            else {
                // receive message from a client 


                // do something (send message to other clients)

                // done reading, remove socket from list of file descriptors
                FD_CLR(i, &read_fds); 

            }
        }
    }
}