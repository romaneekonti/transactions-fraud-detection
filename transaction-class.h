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

    std::string to_tabular_short() {
        //      id      accnt   time    ammnt    crncy
        return "%-15s | %-20s | %-10s | %-8.2f | %-4s |";
    }
    std::string to_tabular() {
        //      id      c_id    accnt   time    mrchnt  ammnt    crncy
        return "%-15s | %-15s | %-20s | %-10s | %-20s | %-12.2f | %-4s |";
    }
    std::string to_tabular_long() {
        //      id      c_id    accnt   time    mrchnt  ammnt    crncy  city    device  chnl
        return "%-15s | %-15s | %-20s | %-10s | %-20s | %-12.2f | %-4s | %-20s | %-15s | %-10s |";
    }

    void print_row(char size = 'm') {
        std::string format = this->to_tabular();
        switch (size) {
            case 's': format = this->to_tabular_short(); break;
            default: case 'm': format = this->to_tabular(); break;
            case 'l': format = this->to_tabular_long(); break;
        }

        printw(
            format.c_str(),
            this->id.c_str(),
            this->customer_id.c_str(),
            this->account.c_str(),
            this->timestamp.c_str(),
            this->merchant.c_str(),
            this->amount,
            this->currency.c_str()
        );
    }

    friend void read_csv_file(std::string file_name, std::vector<Transaction>& data);
};

void read_csv_file(std::string file_name, std::vector<Transaction>& data) {
    std::ifstream f(file_name);

    if (!f.is_open()) return;

    std::string line;
    if (!getline(f, line)) return; //header line

    std::stringstream header_ss(line);
    std::string col_name;
    std::unordered_map<std::string, int> col_index;

    int idx = 0;
    while (getline(header_ss, col_name, ',')) {
        col_index[col_name] = idx++; //saving all column to match the needed ones later
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

#endif