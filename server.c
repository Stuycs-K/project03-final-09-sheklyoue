#include "functions.h" 

int main() {
    char client_names[MAX_CLIENTS][BUFFER_SIZE];
    int client_fds[MAX_CLIENTS];
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
                int client_socket = handle_new_connection(server_socket, client_fds, client_names);
                // add socket to fdset
                FD_SET(client_socket, &read_fds);
                // update max_descriptor 
                if (client_socket > max_descriptor) {
                    max_descriptor = client_socket;
                }
                
                // for (int c = 0; c < MAX_CLIENTS; c++) {
                //     if (client_fds[c] > 0) {
                //         write(client_fds[c], client_fds, sizeof(client_fds));
                //         write(client_fds[c], client_names, sizeof(client_names));
                //     }
                // }
            }
            else {
                // for (int c = 0; c < MAX_CLIENTS; c++) {
                //     if (client_fds[c] > 0) {
                //         write(client_fds[c], client_fds, sizeof(client_fds));
                //         write(client_fds[c], client_names, sizeof(client_names));
                //     }
                // }

                // receive message from a client 
                
                    char message[BUFFER_SIZE];

                    read_from_client(message, i, client_fds, client_names, &read_fds);
                    write_to_clients(message, client_fds);
                

            }
        }
    }
    endwin();
}