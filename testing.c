#include <ncurses.h>

int main() {
    // Initialize ncurses
    initscr();
    cbreak();    // Disable line buffering
    noecho();    // Don't echo input characters
    curs_set(0); // Hide the cursor

    // Create a large pad
    int pad_height = 100, pad_width = 80;
    WINDOW *pad = newpad(pad_height, pad_width);

    // Add some content to the pad
    for (int i = 0; i < pad_height; i++) {
        mvwprintw(pad, i, 0, "This is line %d in the pad", i + 1);
    }

    // Create a smaller window to display part of the pad
     int win_height = 20, win_width = 40;
    // WINDOW *win = newwin(win_height, win_width, 0, 0);

    // Initially show the top-left portion of the pad in the window
    prefresh(pad, 0, 0, 0, 0, win_height, win_width);
    //wrefresh(win); // Refresh the window to show the pad content

    // Scroll through the pad using arrow keys
    int top = 0, left = 0;
    while (1) {
        int ch = getch();
        if (ch == 'q') {
            break;  // Quit if the user presses 'q'
        } else if (ch == KEY_DOWN && top + win_height < pad_height) {
            top++;  // Scroll down
        } else if (ch == KEY_UP && top > 0) {
            top--;  // Scroll up
        } else if (ch == KEY_RIGHT && left + win_width < pad_width) {
            left++;  // Scroll right
        } else if (ch == KEY_LEFT && left > 0) {
            left--;  // Scroll left
        }

        // Update the window with the new portion of the pad
        prefresh(pad, top, left, 0, 0, win_height, win_width);
        //wrefresh(win);  // Refresh the window
    }

    // Clean up and close ncurses
    //delwin(win);
    delwin(pad);
    endwin();
    
    return 0;
}
