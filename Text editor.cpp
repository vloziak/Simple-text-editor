#include <iostream>
#include <fstream> // доступ до класу ofstream, ifstream
#include <string>
#include <cstdlib>
#include <cstring>
#include <stack>

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

Line *head = nullptr;
char* buffer = nullptr;
stack<Line*> undo_stack;
stack<Line*> redo_stack;
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
void undo();
void redo();
Line* clone_text();
void restore_text(Line* number);
void delete_all_text();
void clean_console();
void cut();
void copy(int line_number, int start_idx, int end_idx);
void paste();

int main() {
    int command;
    print_help();
    while (true) {
        cout << "Choose the command:\n";
        cin >> command;
        cin.ignore(); // забираємо '\n'

        if(!(command > -1 && command < 17)) {
            cout << "Invalid command.\n";
            continue;
        }

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
            case 12:
                undo();
            break;
            case 13:
                redo();
            break;
            case 14:
                cut();
            break;
            case 15:
                cout << "Enter the line number, start index, and end index (line_number start_idx end_idx): ";
            int line_number, start_idx, end_idx;
            cin >> line_number >> start_idx >> end_idx;
            copy(line_number,start_idx,end_idx);
            break;
            case 16:
                paste();
            break;
            default:
                cout << "The command is not implemented.\n";

            head = new Line;
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

    undo_stack.push(clone_text());
    while(!redo_stack.empty()) redo_stack.pop(); // очищаємо redoStack

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
    ofstream file("saved_text.txt"); //output file stream
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
    cout << "Text was saved successfully.\n";
}
void load_text_from_file(const char *filename) {
    ifstream file(filename); //input file stream
    if(!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

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
    char mode; // щоб вибрати мод як вставити текст

    cout << "Choose line and index(example: 1 10): ";
    cin >> line >> index;
    cin.ignore(); // видалення \n

    cout << "Enter insert or replace(i or r): ";
    cin >> mode;
    cin.ignore();

    cout << "Enter text to insert: ";
    cin.getline(insert_text,100);
    Line *current = get_line(line);
    if (current == nullptr) {
        cout << "Invalid line number.\n";
        return;
    }

    if (index > strlen(current->text)) {
        cout << "Invalid index.\n";
        return;
    }
    if(mode == 'i') {
        int new_length = strlen(current->text) + strlen(insert_text) + 1;
        char* new_text = new char[new_length];

        strncpy(new_text, current->text, index); //копіювання символів з одного рядка в інший:destination,source,number
        new_text[index] = '\0';
        strcat(new_text, insert_text);
        strcat(new_text, current->text + index);

        delete[] current->text;
        current->text = new_text;
        cout << "Text inserted successfully.\n";
    } else if (mode == 'r') {
        int new_length = strlen(current->text) - index + strlen(insert_text);
        char* new_text = new char[new_length + 1];
        strncpy(new_text, current->text, index);
        new_text[index]= '\0';
        strcat(new_text, insert_text);
        strcat(new_text,current->text + index + strlen(insert_text));
        delete[] current ->text;
        current->text = new_text;
        cout << "Text replaced successfully.\n";
    } else {
        cout << "Invalid mode selected.\n";
    }
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

    for (int i = 0; i < line; ++i) {
        if (current == nullptr) {
            cout << "Invalid line number.\n";
            return;
        }
        current = current->next;
    }

    if (current == nullptr || index > strlen(current->text)) {
        printf("Invalid index.\n");
        return;
    }

    int new_length = strlen(current->text) + strlen(insert_text) + 1;
    char* new_text = new char[new_length];


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
void undo() {
    if(undo_stack.empty()) {
        cout << "Nothing to return to.\n";
        return;
    }

    redo_stack.push(clone_text());

    Line* previous_text = undo_stack.top();
    undo_stack.pop();
    restore_text(previous_text);
    cout << "Returned to previous text.\n";
}

void redo() {
    if(redo_stack.empty()) {
        cout << "Nothing to return to.\n";
        return;
    }

    undo_stack.push(clone_text());

    Line* next = redo_stack.top();
    redo_stack.pop();
    restore_text(next);
    cout << "Redo successfully.\n";
}
Line* clone_text() {
    if (head == nullptr) {
        return nullptr;
    }
    Line* new_head = new Line();
    Line* current = head;
    Line* new_current = new_head;
    while (current != nullptr) {
        delete[] new_current->text;
        new_current->text = new char[strlen(current->text)+ 1];
        strcpy(new_current->text, current->text);
        if(current->next != nullptr) {
            new_current->next = new Line();
        }else {
            new_current->next = nullptr;
        }
        current = current->next;
        new_current = new_current->next;
    }
    return new_head;
}

void restore_text(Line* number) {
    delete_all_text();

    if (number == nullptr) {
        return;
    }

    head = new Line();
    Line* current = number;
    Line* new_current = head;
    while (current != nullptr) {
        delete[] new_current->text;
        new_current->text = new char[strlen(current->text) + 1];
        strcpy(new_current->text, current->text);
        if (current->next != nullptr) {
            new_current->next = new Line();
        } else {
            new_current->next = nullptr;
        }
        current = current->next;
        new_current = new_current->next;
    }
}
void copy(int line_number, int start_idx, int end_idx) {

    Line* line = get_line(line_number);
    if (line == nullptr || start_idx < 0 || end_idx >= strlen(line->text)) {
        cout << "Invalid selection.\n";
        return;
    }

    int length = end_idx - start_idx + 1;
    if (length <= 0) {
        cout << "Invalid selection.\n";
        return;
    }

    buffer = new char[length + 1];
    strncpy(buffer, line->text + start_idx, length);
    buffer[length] = '\0';

    cout << "Text copied to clipboard.\n";
}

void cut() {
    int line_number, start_idx, end_idx;
    cout << "Enter the line number, start index, and end index (line_number start end): ";
    cin >> line_number >> start_idx >> end_idx;

    copy(line_number,start_idx,end_idx);

    Line* current = get_line(line_number);

    if (current == nullptr) {
        cout << "Invalid line number.\n";
        return;
    }
    int text_length = strlen(current->text);
    if (start_idx >= text_length || end_idx >= text_length || start_idx > end_idx) {
        cout << "Invalid start or end index.\n";
        return;
    }

    undo_stack.push(clone_text());
    while (!redo_stack.empty()) redo_stack.pop();

    int new_length = text_length - (end_idx - start_idx + 1);
    char* new_text = new char[new_length + 1];
    strncpy(new_text, current->text, start_idx);
    new_text[start_idx] = '\0';
    strcat(new_text, current->text + end_idx + 1);
    delete[] current->text;
    current->text = new_text;
    cout << "Text deleted successfully.\n";
}

void paste() {
    int line_number, index;
    cout << "Choose line and index to paste the text (line number index number): ";
    cin >> line_number >> index;

    if (buffer == nullptr) {
        cout << "Clipboard is empty.\n";
        return;
    }

    Line* line = get_line(line_number);
    if (line == nullptr) {
        cout << "Invalid line number.\n";
        return;
    }

    int new_length = strlen(line->text) + strlen(buffer) + 1;
    char* new_text = new char[new_length];
    strncpy(new_text, line->text, index);
    new_text[index] = '\0';
    strcat(new_text, buffer);
    strcat(new_text, line->text + index);

    delete[] line->text;
    line->text = new_text;

    cout << "Text pasted successfully.\n";
}

void delete_all_text() {
    Line* current = head;
    while (current != nullptr) {
        Line* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
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
    cout << "6 - Insert or replace text\n";
    cout << "7 - Find longest substring\n";
    cout << "8 - Insert substring\n";
    cout << "9 - Print help\n";
    cout << "10 - Delete all text\n";
    cout << "11 - Clean console\n";
    cout << "12 - Undo\n";
    cout << "13 - Redo\n";
    cout << "14 - Cut\n";
    cout << "15 - Copy\n";
    cout << "16 - Paste\n";
    }