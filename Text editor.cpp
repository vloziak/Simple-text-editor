#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Line {
    char *text;
    struct Line *next;
} Line;

// Pointer for linked list
Line *head = NULL;

void write_text();
void start_new_line();
void print_text();
void insert_text();
void free_memory();
void print_help();

int main() {
    int command;

    print_help();

    // reading commands from user
    while (1) {
        printf("Choose the command:\n");
        scanf("%d", &command); //%d-expecting to get number from user and write it in command
        getchar(); // getting number that user wrote

        switch (command) {
            case 0:
                free_memory();
                exit(0);
            case 1:
                write_text();
                break;
            case 2:
                start_new_line();
                break;
            case 5:
                print_text();
                break;
            case 6:
                insert_text();
                break;
            case 9:
                print_help();
                break;
            default:
                printf("The command is not implemented.\n");
        }
    }

    return 0;
}

void write_text() {
    if (head == NULL) {
        head = (Line *)malloc(sizeof(Line));
        head->text = (char *)malloc(1);
        head->text[0] = '\0';
        head->next = NULL;
    }

    Line *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    char buffer[100];
    printf("Enter your text: ");
    fgets(buffer, 100, stdin); // stdin - зчитування символів з клавіатури(з консолі)
    buffer[strcspn(buffer, "\n")] = '\0'; // замінюємо символ переносу рядка на символ нульового рядка

    int new_length = strlen(current->text) + strlen(buffer) + 1; // +1 для "\0" динамічне збільшення рядка
    current->text = (char *)realloc(current->text, new_length); // переписуємо пам'ять рядка
    strcat(current->text, buffer); // приєднання buffer до current text
}

void start_new_line() {
    Line *new_line = (Line *)malloc(sizeof(Line));
    new_line->text = (char *)malloc(1);
    new_line->text[0] = '\0';
    new_line->next = NULL;

    if (head == NULL) {
        head = new_line;
    } else {
        Line *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_line;
    }

    printf("New line is started.\n");
}

void print_text() {
    Line *current = head;
    while (current != NULL) {
        char *text = current->text;
        while (*text != '\0') {
            putchar(*text);
            text++;
        }
        putchar('\n');
        current = current->next;
    }
}


void insert_text() {
    int line, index;
    char insert_text[100];

    printf("Choose line and index:(line number index number) ");
    scanf("%d %d", &line, &index);
    getchar(); // consume newline character

    printf("Enter text to insert: ");
    fgets(insert_text, 100, stdin);
    insert_text[strcspn(insert_text, "\n")] = '\0'; //strcspn-calculates the length of the number of characters before \n

    Line *current = head;
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
    char *new_text = (char *)malloc(new_length);

    strncpy(new_text, current->text, index);
    new_text[index] = '\0';
    strcat(new_text, insert_text);
    strcat(new_text, &current->text[index]);// &-отримання посилання на комірку в пам'яті

    free(current->text);
    current->text = new_text;

    printf("Text inserted successfully.\n");
}

void free_memory() {
    Line *current = head;
    while (current != NULL) {
        Line *next = current->next; // зберігаємо посилання на наступний елемент
        free(current->text);
        free(current);
        current = next;
    }
}

void print_help() {
    printf("Commands:\n");
    printf("0: Exit\n");
    printf("1: Write your text\n");
    printf("2: Start new line\n");
    printf("3: Print text\n");
    printf("4: Insert text\n");
    printf("5: Help\n");
}
