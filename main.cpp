#include <iostream>
#include <ncurses.h>

using namespace std;

int main() {
    int nr_options = 4;
    initscr();
    keypad(stdscr, true);

    printw("Hello world 2\n");
    printw("Hello world 3\n");

    while (true) {
        int ch = getch();

        if (ch == KEY_UP) {
            printw("up");
        }

        if (ch == 'q') break;
    }
    endwin();

    return 0;    
}