#include <iostream>
#include <fstream> // доступ до класу ofstream, ifstream
#include <string>
#include <windows.h>
#include "class/Text_Editor.cpp"

using namespace std;

int main() {
    HINSTANCE handle = LoadLibrary(TEXT("C:\\Users\\Professional\\CLionProjects\\untitled2\\cmake-build-debug\\libcaesar.dll"));
    if (handle == nullptr || handle == INVALID_HANDLE_VALUE) {
        cout << "Library not found" << endl;
        return 1;
    }

    typedef string (*encrypt_ptr)(string, int);
    typedef string (*decrypt_ptr)(string, int);

    encrypt_ptr encrypt = (encrypt_ptr)GetProcAddress(handle, TEXT("encrypt"));
    if (encrypt == nullptr) {
        cout << "Encrypt function not found" << endl;
        return 1;
    }

    decrypt_ptr decrypt = (decrypt_ptr)GetProcAddress(handle, TEXT("decrypt"));
    if (decrypt == nullptr) {
        cout << "Decrypt function not found" << endl;
        return 1;
    }

    string text;
    cout << "Enter text: ";
    getline(cin, text);
    string encrypted_text = encrypt(text, 12);
    cout << "Encrypted text: " << encrypted_text << endl;

    string code;
    cout << "Enter code: ";
    getline(cin, code);
    string decrypted_text = decrypt(code, 12);
    cout << "Decrypted text: " << decrypted_text << endl;

    FreeLibrary(handle);
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
            default:
                cout << "Invalid command.\n";
        }
    }
    return 0;
}