#include "functions.h"

int main() {
    // printf("line 1\n");
    // printf("line 2\n");
    // printf("line 3\n");
    // printf("line 4\n");
    // printf("line 5\n");

    // clear_terminal();
    // printf("Terminal cleared!\n");
    print_chat("chat.txt");
    sleep(0.5);
    printf("clearing chat\n");
    sleep(2);
    clear_terminal();
    return 0;
}