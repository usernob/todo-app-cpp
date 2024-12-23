#include <chrono>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "fileio.h"
#include "terminal_fancy.h"
#include "todo.h"

using namespace std;

static BinaryFileHanlder todo_db("database.bin");
static TodoList items;
static EscapeSequence fancy;

// clang-format off
static const char box_char[11][4] = {"─", "│", "┌",  "┐", "└", "┘", "├", "┤", "┬", "┴", "┼"};
static const string table_string[10] = {
    "┏━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━┓",
    "┃ id    ┃ name                                     ┃ status        ┃",
    "┡━━━━━━━╇━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╇━━━━━━━━━━━━━━━┩",
    "├───────┼──────────────────────────────────────────┼───────────────┤",
    "└───────┴──────────────────────────────────────────┴───────────────┘"
};
// clang-format on

static constexpr unsigned int title_max_len = 40;
static constexpr unsigned int id_max_len = 6;
static constexpr unsigned int status_max_len = 14;

void get_input(const string &message, string &output) {
    cout << message;
    getline(cin, output);
}

void show_message(const string &message, const char *format) {
    string temp;
    cout << fancy.escape(message.c_str(), format)
         << "(tekan enter untuk melanjutkan)";
    getline(cin, temp);
}

void print_row(const TodoItem &item, const bool is_firstrow) {
    // cara mendapatkan jumlah digit dari sebuah angka
    // reference:
    // https://stackoverflow.com/questions/1306727/way-to-get-number-of-digits-in-an-int
    const size_t digit_len = (size_t)log10(item.id) + 1;

    cout << box_char[1] << ' ';

    // jika is_firstrow maka print id dan padding, jika tidak maka print padding
    // saja
    if (is_firstrow) {
        cout << item.id << string(id_max_len - digit_len, ' ');
    } else {
        cout << string(id_max_len, ' ');
    }

    // print title
    cout << box_char[1] << ' ' << item.title
         << string((title_max_len - item.title.length()) + 1, ' ')
         << box_char[1] << ' ';

    // jika is_firstrow maka print status dan padding, jika tidak maka print
    // padding saja
    if (is_firstrow) {
        string status =
            item.complete
                ? fancy.escape("selesai", EscapeSequence::Modifier::GREEN)
                : fancy.escape("belum selesai", EscapeSequence::Modifier::RED);
        // maksimal panjang status adalah status_max_len,
        // 7 adalah status_max_len - length "selesai"
        cout << status << string(item.complete ? 7 : 1, ' ');  // NOLINT
    } else {
        cout << string(status_max_len, ' ');
    }

    // print penutup table
    cout << box_char[1] << "\n";
}

void print_table_todo() {
    cout << table_string[0] << "\n"
         << table_string[1] << "\n"
         << table_string[2] << "\n";

    if (items.is_empty()) {
        cout << table_string[4] << "\n";
        return;
    }

    for (auto it = items.begin(); it != items.end(); ++it) {
        const TodoItem item = *it;
        size_t line_counter = 0;

        size_t prev_pos = 0;
        size_t pos = 0;
        size_t start = 0;

        TodoItem new_item = {item.id, item.title, item.complete};
        if (item.title.length() <= title_max_len) {
            print_row(item, true);
        } else {
            while ((pos = item.title.find(' ', prev_pos + 1)) != string::npos) {
                if (pos - start > title_max_len) {
                    new_item.title = item.title.substr(start, prev_pos - start);
                    print_row(new_item, line_counter == 0);

                    start = prev_pos + 1;
                    line_counter++;
                }
                prev_pos = pos;
            }

            // print kalimat yang tersisa
            new_item.title =
                item.title.substr(start, item.title.length() - start);
            print_row(new_item, false);
        }

        // jika item terakhir
        if (it.current->next == nullptr) {
            cout << table_string[4] << "\n";
        } else {
            cout << table_string[3] << "\n";
        }
    }
}

void add_todo() {
    string title;
    auto now = std::chrono::system_clock::now();
    auto epoch =
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
            .count();
    get_input("Masukkan judul todo : ", title);

    TodoItem *last_todo = items.bot();
    // jika nullptr atau kosong maka set last_id ke 1
    unsigned int last_id = last_todo == nullptr ? 1 : last_todo->id;

    unsigned int new_id = ((last_id ^ epoch) + 1) % 10000;  // NOLINT

    TodoItem data{new_id, title, false};
    items.append(data);
    show_message("Todo berhasil ditambahkan", EscapeSequence::Modifier::GREEN);
}

bool get_id(unsigned int &todo_id) {
    string id_str;
    get_input("Masukkan id todo : ", id_str);

    try {
        todo_id = stoi(id_str);
    } catch (exception &e) {
        show_message("id yang anda masukkan tidak valid",
                     EscapeSequence::Modifier::RED);
        return false;
    }
    return true;
}

void update_todo() {
    unsigned int todo_id;

    if (!get_id(todo_id)) {
        return;
    }

    TodoItem *item = items.search_id(todo_id);
    if (item == nullptr) {
        show_message("id tidak ditemukan", EscapeSequence::Modifier::RED);
        return;
    }
    cout << "Judul : " << item->title << "\n";
    get_input("Masukkan judul baru : ", item->title);
    show_message("Todo berhasil di update", EscapeSequence::Modifier::GREEN);
}

void delete_todo() {
    unsigned int todo_id;

    if (!get_id(todo_id)) {
        return;
    }

    if (items.delete_id(todo_id)) {
        show_message("Todo telah dihapus", EscapeSequence::Modifier::GREEN);
        return;
    }
    show_message("id tidak ditemukan", EscapeSequence::Modifier::RED);
}

void toggle_status() {
    unsigned int todo_id;
    if (!get_id(todo_id)) {
        return;
    }

    TodoItem *item = items.search_id(todo_id);
    if (item == nullptr) {
        show_message("id tidak ditemukan", EscapeSequence::Modifier::RED);
        return;
    }

    item->complete = !item->complete;
}

int proccess_choice(const char &choice) {
    switch (choice) {
        case 'T':
        case 't':
            add_todo();
            break;
        case 'U':
        case 'u':
            update_todo();
            break;
        case 'H':
        case 'h':
            delete_todo();
            break;
        case 'S':
        case 's':
            toggle_status();
            break;
        case 'Q':
        case 'q':
            return 1;
            break;
    }
    return 0;
}

void print_menu() {
    while (true) {
        // clear terminal
#if defined(_WIN32)
        system("cls");
#elif defined(__linux__) || defined(__unix__)
        cout << "\x1B[2J\x1B[H";
#endif

        cout << fancy.escape("Your Todo List", EscapeSequence::Modifier::BLUE)
             << "\n";
        print_table_todo();

        string choice;
        // NOLINTBEGIN
        string T = fancy.escape("T", EscapeSequence::Modifier::CYAN);
        string H = fancy.escape("H", EscapeSequence::Modifier::RED);
        string U = fancy.escape("U", EscapeSequence::Modifier::BLUE);
        string S = fancy.escape("S", EscapeSequence::Modifier::GREEN);
        string Q = fancy.escape("Q", EscapeSequence::Modifier::RED);
        cout << fancy.escape("Menu", EscapeSequence::Modifier::BLUE) << "\n";
        cout << "- [" << T << "]ambah Todo\n";
        cout << "- [" << H << "]apus Todo\n";
        cout << "- [" << U << "]pdate Todo\n";
        cout << "- Toggle [" << S << "]elesai\n";
        cout << "- [" << Q << "]uit\n";
        cout << "Masukkan Pilihan";
        get_input("[" + T + "/" + H + "/" + U + "/" + S + "/" + Q + "] : ",
                  choice);
        // NOLINTEND

        int break_status = proccess_choice(*choice.c_str());
        if (break_status != 0) {
            break;
        }
    }
}
void sigint_handler(int signal) {
    // Cleanup kode di sini
    cout << "\nlain kali ketik q untuk keluar" << endl;
    todo_db.write(items);
    todo_db.~BinaryFileHanlder();
    // clear resource
    items.~TodoList();
    exit(0);
}

int main() {
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
#endif
    signal(SIGINT, sigint_handler);
    todo_db.read(items);

    print_menu();
    todo_db.write(items);
    return 0;
}
