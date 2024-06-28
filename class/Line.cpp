#include "Line.h"

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