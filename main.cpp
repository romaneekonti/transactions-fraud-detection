#include <iostream>
#include <filesystem>
#include <fstream>
#include <ncurses.h>
#include <vector>

#include "transaction-class.h"

using namespace std;

void show_title(string title, int color_pair = 0) {
    int row, col;
    getmaxyx(stdscr, row, col);

    int start_col = (col - title.length()) / 2;
    title += "\n";
    
    attron(COLOR_PAIR(color_pair));
    mvprintw(0, start_col, title.c_str());
    attroff(COLOR_PAIR(color_pair));
    
    refresh();
}

string ask_csv_file() {
    vector<string> file_array;

    for (const auto& entry : filesystem::directory_iterator(".")) {
        string filename = entry.path().filename().string();

        if (entry.path().extension() == ".csv") file_array.push_back(filename);
    }
    if (file_array.empty()) return "";

    int selected_index = 0;
    string return_value = "";
    
    while (true) {
        clear();
        show_title("Please select a .csv file");
        for (int i = 0; i < file_array.size(); i++) {
            if (i == selected_index) {
                attron(COLOR_PAIR(1));
                printw("[%s]", file_array[i].c_str());
                attroff(COLOR_PAIR(1));
            } else {
                printw("[%s]", file_array[i].c_str());
            }
            printw("\n");
        }
        refresh();

        int c = getch();

        if (c == KEY_UP && selected_index > 0) selected_index--;
        if (c == KEY_DOWN && selected_index < file_array.size() - 1) selected_index++;

        if (c == 'q') break;
        if (c == '\n') {
            return_value = file_array[selected_index];
            break;
        }

    }

    return return_value;
}

void apply_ncurses_config() {
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    mousemask(0, NULL); //disable clicks

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1); //green text, default bg
    init_pair(2, COLOR_CYAN, -1);
}

int main() {
    initscr();
    apply_ncurses_config();

    // string file = ask_csv_file();
    // clear();
    // show_title(file, 2);

    // getch();

    // endwin();

    vector<Transaction> transactions;
    read_csv_file("fake_data.csv", transactions);

    for (int i = 0; i < transactions.size(); i++) {
        transactions[i].print_row();
        printw("\n");
    }
    getch();

    return 0;    
}