#include <iostream>
#include <filesystem>
#include <ncurses.h>
#include <vector>

using namespace std;

void show_file_name_title(string title, string file_name) {
    int row, col;
    getmaxyx(stdscr, row, col);

    int start_col = (col - title.length() - file_name.length()) / 2;
    mvprintw(0, start_col, title.c_str());
    
    attron(COLOR_PAIR(1));
    mvprintw(0, start_col + title.length(), file_name.c_str());
    attroff(COLOR_PAIR(1));
    
    refresh();
}

string ask_file_name() {
    vector<string> file_array;

    for (const auto& entry : filesystem::directory_iterator(".")) {
        string filename = entry.path().filename().string();
        cout << filename << "\n";    // optional: print
        file_array.push_back(filename);
    }

    int selected_index = 0;
    string return_value = "";

    while (true) {
        clear();
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

int main() {
    initscr();
    keypad(stdscr, true);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1); //green text, default bg

    string file = ask_file_name();
    clear();
    show_file_name_title("Selected file: ", file);

    getch();

    endwin();
    return 0;    
}