#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

class Line {
public:
    char* text;
    Line* next;

    Line() {
        text = new char[1];
        text[0] = '\0';
        next = nullptr;
    }
    ~Line() {
        delete[] text;
    }
};

// поінтер
Line *head = nullptr;
const char *filename = "C:\\Users\\Professional\\Desktop\\оп\\assignment-four-hanna_bila_victoria_loziak\\sherlock.txt";

void write_text();
void start_new_line();
void print_text();
void save_text_to_file();
void insert_text();
void free_memory();
void print_help();
void load_text_from_file(const char *filename);
void find_longest_substring();
void insert_substring();
void delete_all_text();
void clean_console();

int main() {
    int command;

    print_help();

    while (true) {
        cout << "Choose the command:\n";
        cin >> command;
        cin.ignore(); // To remove the newline character left in the input buffer

        switch (command) {
            case 0:
                return 0;
            case 1:
                write_text();
                break;
            case 2:
                start_new_line();
                break;
            case 3:
                print_text();
                break;
            case 4:
                save_text_to_file();
                break;
            case 5: {
                load_text_from_file(filename);
                break;
            }
            case 6:
                insert_text();
                break;
            case 7:
                find_longest_substring();
                break;
            case 8:
                insert_substring();
                break;
            case 9:
                print_help();
                break;
            case 10:
                delete_all_text();
                break;
            case 11:
                clean_console();
                break;
            default:
                cout << "The command is not implemented.\n";
        }
    }
}

Line* get_line(int line_number) {
    Line* current = head;
    for (int i = 0; i < line_number; ++i) {
        if (current == nullptr) {
            return nullptr;
        }
        current = current->next;
    }
    return current;
}


void write_text() {
    if (head == nullptr) {
        head = new Line;
    }

    Line* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }

    char buffer[100];
    cout << "Enter your text: ";
    cin.getline(buffer, 100);

    int new_length = strlen(current->text) + strlen(buffer) + 1; // +1 для "\0" динамічне збільшення рядка
    char* new_text = new char[new_length]; // переписуємо пам'ять рядка
    strcpy(new_text, current->text);
    strcat(new_text, buffer); // приєднання buffer до new text

    delete[] current->text;
    current->text = new_text;
}

void start_new_line() {
    Line* new_line = new Line();

    if (head == nullptr) {
        head = new_line;
    } else {
        Line *current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new_line;
    }

    cout << "New line is started.\n";
}

void print_text() {
    Line* current = head;
    while (current != nullptr) {
        cout << current->text << '\n';
        current = current->next;
    }
}

void save_text_to_file() {
    ofstream file("saved_text.txt");
    if(!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

    Line* current = head;
    while(current != nullptr) {
        file << current->text << '\n';
        current = current->next;
    }

    file.close();
    cout <<"Text was saved successfully.\n";
}

void load_text_from_file(const char *filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

    delete_all_text();
    char buffer[100];
    while(file.getline(buffer, 100)) {
        Line* new_line = new Line();
        delete[] new_line->text;
        new_line->text = new char[strlen(buffer)+1];//текст з буферу копіюється в text
        strcpy(new_line->text,buffer);
        if(head == nullptr) {
            head = new_line;
        }
        else {
            Line* current = head;
            while(current->next != nullptr) {
                current = current->next;
            }
            current->next = new_line;
        }
    }
    file.close();
    cout << "Text loaded successfully.\n";
}

void insert_text() {
    int line, index;
    char insert_text[100];

    cout << "Choose line and index:(line number index number)";
    cin >> line >> index;
    cin.ignore(); // видалення \n

    cout << "Enter text to insert: ";
    cin.getline(insert_text,100);

    Line *current = get_line(line);
    for (int i = 0; i < line; ++i) {
        if (current == nullptr) {
            cout << "Invalid line number.\n";
            return;
        }
        current = current->next;
    }

    if (current == nullptr || index > strlen(current->text)) {
        cout << "Invalid index.\n";
        return;
    }

    int new_length = strlen(current->text) + strlen(insert_text) + 1;
    char* new_text = new char[new_length];

    strncpy(new_text, current->text, index);
    new_text[index] = '\0';
    strcat(new_text, insert_text);
    strcat(new_text, current->text + index);// &-отримання посилання на комірку в пам'яті

    delete[] current->text;
    current->text = new_text;

    cout << "Text inserted successfully.\n";
}

void find_longest_substring() {
    char substring[100];
    cout << "Enter the substring to search for: ";
    cin.getline(substring,100);

    int line_number = 0;
    int found = 0;

    Line* current = head;
    while (current != nullptr) {
        char* text = current->text;
        char* match = strstr(text, substring);//повертає індекс match

        while (match != nullptr) {
            found = 1;
            int index = match - text; //різниця між адресами
            cout << "Match found at line:" << line_number << '\n';
            cout << "Match found at index:" << index << '\n';
            match = strstr(match + 1, substring); // Продовжуємо шукати після першого знайденого
        }

        current = current->next;
        ++line_number;
    }

    if (!found) {
        cout << "No matches found.\n";
    }
}

void insert_substring() {
    int line, index;
    char insert_text[100];

    cout << "Choose line and index to insert the substring (line number index number): ";
    cin >> line >> index;
    cin.ignore();

    cout << "Enter the substring to insert: ";
    cin.getline(insert_text,100);

    Line* current = get_line(line);

    for (int i = 0; i < line; i++) {
        if (current == NULL) {
            printf("Invalid line number.\n");
            return;
        }
        current = current->next;
    }

    if (current == NULL || index > strlen(current->text)) {
        printf("Invalid index.\n");
        return;
    }

    int new_length = strlen(current->text) + strlen(insert_text) + 1;
    char *new_text = new char[new_length];


    strncpy(new_text, current->text, index);
    new_text[index] = '\0';

    strcat(new_text, insert_text);

    strcat(new_text, current->text + index);
    new_text[index]= '\0';
    strcat(new_text, insert_text);
    strcat(new_text,current ->text + index);

    delete[] current->text;
    current->text = new_text;

    cout << "Substring inserted successfully.\n";
}

void delete_all_text() {
    Line* current = head;
    while (current != nullptr) {
        Line* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    cout << "All text deleted.\n";
}

void clean_console() {
    system("cls");
    cout << "Console cleare.\n";
}

void print_help() {
    cout << "Commands:\n";
    cout << "0 - Exit\n";
    cout << "1 - Write text\n";
    cout << "2 - Start new line\n";
    cout << "3 - Print text\n";
    cout << "4 - Save text to file\n";
    cout << "5 - Load text from file\n";
    cout << "6 - Insert text\n";
    cout << "7 - Find longest substring\n";
    cout << "8 - Insert substring\n";
    cout << "9 - Print help\n";
    cout << "10 - Delete all text\n";
    cout << "11 - Clean console\n";
}
