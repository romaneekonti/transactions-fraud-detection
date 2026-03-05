#include <iostream>
#include <filesystem>
#include <ncurses.h>
#include <vector>

using namespace std;

string askFileName() {
    vector<string> file_array;

    for (const auto& entry : filesystem::directory_iterator(".")) {
        string filename = entry.path().filename().string();
        cout << filename << "\n";    // optional: print
        file_array.push_back(filename);
    }

    int selected_index = 0;
    string return_value = "";

    initscr();
    keypad(stdscr, true);
    while (true) {
        clear();
        for (int i = 0; i < file_array.size(); i++) {
            if (i == selected_index)
                printw(">>[%s]", file_array[i].c_str());
            else
                printw("[%s]", file_array[i].c_str());
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
    endwin();

    return return_value;
}

int main() {
    
    printf("%s", askFileName().c_str());

    return 0;    
}