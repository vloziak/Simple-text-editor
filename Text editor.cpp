#include <iostream>
#include <fstream> // доступ до класу ofstream, ifstream
#include <string>
#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <stack>

using namespace std;



class Text {
public:
    char* text;
    Text* next;
    Text() {
        text = new char[1];
        text[0] = '\0';
        next = nullptr;
    }
    ~Text() {
        delete[] text;
    }

    Text* convertStringToLinkedList(string& str) {
        Text* head = nullptr;
        Text* current = nullptr;

        for (int i = 0; str[i] != '\0'; ++i) {
            Text* newNode = new Text();

            newNode->text = new char[2];
            newNode->text[0] = str[i];
            newNode->text[1] = '\0';

            if (head == nullptr) {
                head = newNode;
                current = head;
            } else {
                current->next = newNode;
                current = newNode;
            }
        }
       return head;
    }
};

char* convertToString(Text* head) {
    int length = 0;
    Text* current = head;
    while (current != nullptr) {
        length += strlen(current->text);
        current = current->next;
    }

    // Allocate memory for the resulting string
    char* result = new char[length + 1]; // +1 for the null terminator
    result[0] = '\0'; // Initialize the result string

    // Second pass to concatenate the characters into the result string
    current = head;
    while (current != nullptr) {
        strcat(result, current->text);
        current = current->next;
    }
    return result;
}

class Caesar {
    HINSTANCE handle;
public:
    Caesar() {
        handle = LoadLibrary(TEXT("C:\\Users\\Professional\\Desktop\\оп\\Simple-text-editor\\libcaesar.dll"));
        if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
            cout << "Library not found" << endl;
        }

    }
    Text* encryptText(string rawText, int key){
        typedef string (*encrypt_ptr)(string, int);

        encrypt_ptr encrypt = (encrypt_ptr)GetProcAddress(handle, TEXT("encrypt"));
        if (encrypt == nullptr) {
            cout << "Encrypt function not found" << endl;
        }

        string encrypted_text = encrypt(rawText, key);
        return convertStringToLinkedList(encrypted_text);
    }

    Text* decrypt(string rawText, int key){
        typedef string (*decrypt_ptr)(string, int);

        decrypt_ptr decrypt = (decrypt_ptr)GetProcAddress(handle, TEXT("decrypt"));
        if (decrypt == nullptr) {
            cout << "Decrypt function not found" << endl;
        }

        string decrypted_text = decrypt(rawText, key);
        return convertStringToLinkedList(decrypted_text);
    }
private:
    Text* convertStringToLinkedList(string& str) {
        Text* head = nullptr;
        Text* current = nullptr;

        for (int i = 0; str[i] != '\0'; ++i) {
            Text* newNode = new Text();

            newNode->text = new char[2];
            newNode->text[0] = str[i];
            newNode->text[1] = '\0';

            if (head == nullptr) {
                head = newNode;
                current = head;
            } else {
                current->next = newNode;
                current = newNode;
            }
        }
        return head;
    }
};


class TextEditor {
    Text* head;
    char* buffer;
    stack<Text*> undo_stack;
    stack<Text*> redo_stack;

public:
    TextEditor() {
        head = nullptr;
    }

    ~TextEditor() {
        delete_all_text();
    }

    void write_text() {
        if (head == nullptr) {
            head = new Text;
        }

        undo_stack.push(clone_text());
        while(!redo_stack.empty()) redo_stack.pop(); // очищаємо redoStack

        Text* current = head;
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

    Text* get_line(int line_number) {
        Text* current = head;
        for (int i = 0; i < line_number; ++i) {
            if (current == nullptr) {
                return nullptr;
            }
            current = current->next;
        }
        return current;
    }

    void start_new_line() {
        Text* new_line = new Text();
        if (head == nullptr) {
            head = new_line;
        } else {
            Text *current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new_line;
        }
        cout << "New line is started.\n";
    }

    void print_text() {
        Text* current = head;
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
        Text* current = head;
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
            Text* new_line = new Text();
            delete[] new_line->text;
            new_line->text = new char[strlen(buffer)+1];//текст з буферу копіюється в text
            strcpy(new_line->text,buffer);
            if(head == nullptr) {
                head = new_line;
            }
            else {
                Text* current = head;
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
        Text *current = get_line(line);
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
        Text* current = head;
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

        Text* current = get_line(line);

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

        Text* previous_text = undo_stack.top();
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

        Text* next = redo_stack.top();
        redo_stack.pop();
        restore_text(next);
        cout << "Redo successfully.\n";
    }

    Text* clone_text() {
        if (head == nullptr) {
            return nullptr;
        }
        Text* new_head = new Text();
        Text* current = head;
        Text* new_current = new_head;
        while (current != nullptr) {
            delete[] new_current->text;
            new_current->text = new char[strlen(current->text)+ 1];
            strcpy(new_current->text, current->text);
            if(current->next != nullptr) {
                new_current->next = new Text();
            }else {
                new_current->next = nullptr;
            }
            current = current->next;
            new_current = new_current->next;
        }
        return new_head;
    }

    void restore_text(Text* number) {
        delete_all_text();

        if (number == nullptr) {
            return;
        }

        head = new Text();
        Text* current = number;
        Text* new_current = head;
        while (current != nullptr) {
            delete[] new_current->text;
            new_current->text = new char[strlen(current->text) + 1];
            strcpy(new_current->text, current->text);
            if (current->next != nullptr) {
                new_current->next = new Text();
            } else {
                new_current->next = nullptr;
            }
            current = current->next;
            new_current = new_current->next;
        }
    }

    void copy(int line_number, int start_idx, int end_idx) {

        Text* line = get_line(line_number);
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

        Text* current = get_line(line_number);

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

        Text* line = get_line(line_number);
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
        Text* current = head;
        while (current != nullptr) {
            Text* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }


    void encrypt(int key) {
        Caesar caesar;
        head = caesar.encryptText(convertToString(head), key);
    }

    void encryptFile(char inFile[25], char outFile[25], int key) {
        Caesar cipher;
        size_t chunkSize = 128;

        ifstream inputFile(inFile, ios::binary);
        ofstream outputFile(outFile, ios::binary);
        char* buffer = new char[chunkSize];

        if (!inputFile.is_open() || !outputFile.is_open()) {
            cout << "Failed to open input or output file." << endl;
            delete[] buffer;
        }

        while (!inputFile.eof()) {
            inputFile.read(buffer, chunkSize);
            streamsize bytesRead = inputFile.gcount();

            if (bytesRead > 0) {
                string rawText(buffer, bytesRead);
                Text* encryptedText = cipher.encryptText(rawText, key);

                int length = 0;
                Text* current = encryptedText;
                while (current != nullptr) {
                    length += strlen(current->text);
                    current = current->next;
                }

                // Allocate memory for the resulting string
                char* result = new char[length]; // +1 for the null terminator
                result[0] = '\0'; // Initialize the result string

                // Second pass to concatenate the characters into the result string
                current = encryptedText;
                while (current != nullptr) {
                    strcat(result, current->text);
                    current = current->next;
                }

                outputFile.write(result, length);
            }
        }

        delete[] buffer;
        inputFile.close();
        outputFile.close();
    }

    void decryptFile(char inFile[25], char outFile[25], int key) {
        Caesar cipher;
        size_t chunkSize = 128;

        ifstream inputFile(inFile, ios::binary);
        ofstream outputFile(outFile, ios::binary);
        char* buffer = new char[chunkSize];

        if (!inputFile.is_open() || !outputFile.is_open()) {
            cout << "Failed to open input or output file." << endl;
            delete[] buffer;
        }

        while (!inputFile.eof()) {
            inputFile.read(buffer, chunkSize);
            streamsize bytesRead = inputFile.gcount();

            if (bytesRead > 0) {
                string rawText(buffer, bytesRead);
                Text* encryptedText = cipher.decrypt(rawText, key);

                int length = 0;
                Text* current = encryptedText;
                while (current != nullptr) {
                    length += strlen(current->text);
                    current = current->next;
                }

                // Allocate memory for the resulting string
                char* result = new char[length]; // +1 for the null terminator
                result[0] = '\0'; // Initialize the result string

                // Second pass to concatenate the characters into the result string
                current = encryptedText;
                while (current != nullptr) {
                    strcat(result, current->text);
                    current = current->next;
                }

                outputFile.write(result, length);
            }
        }

        delete[] buffer;
        inputFile.close();
        outputFile.close();
    }

    void decrypt(int key) {
        Caesar caesar;
        head = caesar.decrypt(convertToString(head), key);
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
        cout << "17 - Encrypt\n";
        cout << "18 - Decrypt\n";
        cout << "19 - Decrypt from one file and encrypt in another\n";
        cout << "20 - Encrypt from one file and decrypt in another\n";
    }
};


int main() {
    TextEditor editor;
    editor.print_help();

    while (true) {
        int command;
        cout << "Choose the command:\n";
        cin >> command;
        cin.ignore();

        switch (command) {
            case 0:
                return 0;
            case 1:
                editor.write_text();
            break;
            case 2:
                editor.start_new_line();
            break;
            case 3:
                editor.print_text();
            break;
            case 4:
                editor.save_text_to_file();
            break;
            case 5:
                editor.load_text_from_file("C:\\Users\\Professional\\Desktop\\оп\\assignment-four-hanna_bila_victoria_loziak\\sherlock.txt");
            break;
            case 6:
                editor.insert_text();
            break;
            case 7:
                editor.find_longest_substring();
            break;
            case 8:
                editor.insert_substring();
            break;
            case 9:
                editor.print_help();
            break;
            case 10:
                editor.delete_all_text();
            break;
            case 11:
                editor.clean_console();
            break;
            case 12:
                editor.undo();
            break;
            case 13:
                editor.redo();
            break;
            case 14:
                editor.cut();
            break;
            case 15:
                int line_number, start_idx, end_idx;
            cout << "Enter the line number, start index, and end index: ";
            cin >> line_number >> start_idx >> end_idx;
            editor.copy(line_number, start_idx, end_idx);
            break;
            case 16:
                editor.paste();
            break;
            case 17:
                int key;
                cout << "Enter key: ";
                cin >> key;
                editor.encrypt(key);
            break;
            case 18:
                int key1;
                cout << "Enter key: ";
                cin >> key1;
                editor.decrypt(key1);
            break;
            case 19:
                int key2;
                char inFile[25];
                char outFile[25];
                cout << "Enter input and output files and key: ";
                cin >> inFile >> outFile >> key2;
                editor.encryptFile(inFile, outFile, key2);
            break;
            case 20:
                int key3;
                char inFile1[25];
                char outFile1[25];
                cout << "Enter input and output files and key: ";
                cin >> inFile1 >> outFile1 >> key3;
                editor.decryptFile(inFile1, outFile1, key3);
            break;
            default:
                cout << "Invalid command.\n";
        }
    }
    return 0;
}