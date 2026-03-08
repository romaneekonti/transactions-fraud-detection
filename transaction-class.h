#ifndef TRANSACTION_CLASS
#define TRANSACTION_CLASS
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <ncurses.h>

class Transaction {
    public:

    std::string id, customer_id;
    std::string account, timestamp, merchant;
    
    double amount;
    std::string currency;

    std::string city, device, channel;

    Transaction() {
        this->id = "";
        this->customer_id = "";
        this->account = "";
        this->timestamp = "";
        this->merchant = "";
        this->amount = 0;
        this->currency = "";
        this->city = "";
        this->device = "";
        this->channel = "";
    }

    static std::string to_tabular_short() {
        //      id      accnt   time    ammnt    crncy
        return "%-15s | %-20s | %-10s | %-8.2f | %-4s |";
    }
    static std::string to_tabular() {
        //      id      c_id    accnt   time    mrchnt  ammnt    crncy
        return "%-15s | %-15s | %-20s | %-10s | %-25s | %-12.2f | %-4s |";
    }
    static std::string to_tabular_long() {
        //      id      c_id    accnt   time    mrchnt  ammnt    crncy  city    device  chnl
        return "%-15s | %-15s | %-20s | %-10s | %-25s | %-12.2f | %-4s | %-20s | %-15s | %-10s |";
    }

    friend void read_csv_file(std::string file_name, std::vector<Transaction>& data);
    friend void print_transaction_table(std::vector<Transaction> data, char table_size, int color_pair);
};

void read_csv_file(std::string file_name, std::vector<Transaction>& data) {
    std::ifstream f(file_name);

    if (!f.is_open()) return;

    std::string line;
    if (!getline(f, line)) return; // header line

    std::stringstream header_ss(line);
    std::string col_name;
    std::unordered_map<std::string, int> col_index;

    int idx = 0;
    while (getline(header_ss, col_name, ',')) {
        col_index[col_name] = idx++; // saving all column to match the needed ones later
    }

    while (getline(f, line)) {
        std::stringstream ss(line);
        std::string cell;
        Transaction t;

        int col = 0;
        while (getline(ss, cell, ',')) {
            for (const auto& [name, i] : col_index) {
                if (i != col) continue;

                if (name == "transaction_id") t.id = cell;
                else if (name == "customer_id") t.customer_id = cell;
                else if (name == "card_number") t.account = cell;
                else if (name == "timestamp") t.timestamp = cell;
                else if (name == "merchant") t.merchant = cell;
                else if (name == "amount") t.amount = std::stod(cell);
                else if (name == "currency") t.currency = cell;
                else if (name == "city") t.city = cell;
                else if (name == "device") t.device = cell;
                else if (name == "channel") t.channel = cell;

                break;
            }
            col++;
        }

        data.push_back(t);
    }

    f.close();
}

void print_transaction_table(std::vector<Transaction> data, char table_size = 'm', int color_pair = 0) {
    int pad_rows = data.size() + 1;
    int pad_cols = 200;
    WINDOW* pad = newpad(pad_rows, pad_cols);

    std::string format = Transaction::to_tabular();
        switch (table_size) {
            case 's': format = Transaction::to_tabular_short(); break;
            default: case 'm': format = Transaction::to_tabular(); break;
            case 'l': format = Transaction::to_tabular_long(); break;
        }

    int start_row = 0, start_col = 0;
    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);

    int c;
    while (true) {
        werase(pad);

        for (int i = 0; i < data.size(); i++) {
            if (i == start_row + screen_height / 2) {
                // highlight center row
                wattron(pad, A_BOLD);
                wattron(pad, COLOR_PAIR(color_pair));
            }

            wprintw(pad,
                format.c_str(),
                data[i].id.c_str(),
                data[i].customer_id.c_str(),
                data[i].account.c_str(),
                data[i].timestamp.c_str(),
                data[i].merchant.c_str(),
                data[i].amount,
                data[i].currency.c_str(),
                data[i].city.c_str(),
                data[i].device.c_str(),
                data[i].channel.c_str()
            );
            wprintw(pad, "\n");

            if (i == start_row + screen_height / 2) {
                wattroff(pad, A_BOLD);
                wattroff(pad, COLOR_PAIR(color_pair));
            }
        }

        prefresh(pad, start_row, start_col, 0, 0, screen_height-1, screen_width-1);

        c = getch();
        if (c == 27) break; // esc
        if (c == KEY_UP && start_row > 0) start_row--;
        if (c == KEY_DOWN && start_row < pad_rows - screen_height) start_row++;
        if (c == KEY_LEFT && start_col > 0) start_col--;
        if (c == KEY_RIGHT && start_col < pad_cols - screen_width) start_col++;
    }

    delwin(pad);
}

#endif