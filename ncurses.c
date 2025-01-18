#include "functions.h"
#include <ncurses.h>


int main() {
  initscr();

  noecho();

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);

  attron(COLOR_PAIR(1));
  printw("hello wlrd\n");

  refresh();

  getch();

  endwin();

  return 0;
}