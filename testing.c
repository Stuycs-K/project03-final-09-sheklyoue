#include "functions.h"

char client_names[MAX_CLIENTS][BUFFER_SIZE];
int client_fds[MAX_CLIENTS];

int main() {    

    client_fds[0] = 4;
    client_fds[1] = 4;
    client_fds[2] = 4;

    strcpy(client_names[0], "Steve");
    strcpy(client_names[1], "Bob");
    strcpy(client_names[2], "Law");

    WINDOW *chat_win = create_chat_win();
    WINDOW *user_win = create_user_win();
    WINDOW *message_win = create_message_win();

    update_user_win(user_win);

    /*
    initscr();            // Initialize ncurses
    cbreak();             // Disable line buffering
    noecho();             // Disable echoing of typed characters
    keypad(stdscr, TRUE); // Enable special keys like arrow keys

    // Create a main window
    int height = 10, width = 40, starty = 5, startx = 10;
    WINDOW *main_win = newwin(height, width, starty, startx);
    refresh();
    box(main_win, 0, 0); 
    wrefresh(main_win);

    
    // Create a subwindow within the main window
    int sub_height = 4, sub_width = 40, sub_y = 15, sub_x = 10;
    WINDOW *sub_win = newwin(sub_height, sub_width, sub_y, sub_x);
    refresh();
    box(sub_win, 0, 0);    // Draw a border around the subwindow
    wrefresh(sub_win);     // Refresh the subwindow

    // Display some text in the subwindow
    mvwprintw(sub_win, 1, 1, "Type a Message: ");
    wrefresh(sub_win);     // Refresh the subwindow to display the text
    
    WINDOW *user = newwin(14, 20, 5, 50);
    refresh();
    box(user, 0, 0);
    wrefresh(user);
    mvwprintw(user, 1, 1, "User List: ");
    wrefresh(user);
    
    // Wait for user input
    getch();

    // Clean up
    delwin(sub_win);  // Delete the subwindow
    delwin(main_win); // Delete the main window
    delwin(user);
    endwin();         // End ncurses mode
    */
    return 0;
}

