#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
using namespace std;

bool checkTitle(char* title) {
    bool word;
    for (int i = 0; title[i] != '\0'; ++i) {
        if (isspace(title[i])) {
            word = true;
        } else if (word) {
            if (!isupper(title[i])) {
                return false;
            }
            word = false;
        }
    }
    return true;
}

int main() {
    char title[256];
    cout << "Enter title:\n";
    cin >> title;
    cin.ignore();
    if (checkTitle(title)) {
        cout << "The title is correct.\n";
    } else {
        cout << "The title is incorrect.\n";
    }
}

