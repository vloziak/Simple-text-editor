#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Line {
    char *text;
    struct Line *next;
} Line;

// поінтер
Line *head = NULL;
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
                printf("The command is not implemented.\n");
        }
    }
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

void write_text() {
    if (head == NULL) {
        head = (Line *)malloc(sizeof(Line));
        head->text = (char *)malloc(1);//виділяє пам'ять для зберігання рядка, здатного вмістити лише один символ(\0)
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

void save_text_to_file() {
    FILE *file = fopen("saved_text.txt", "w");
    if(file == NULL) {
        printf("Error opening file.\n");
    }

    Line *current = head;
    while(current != NULL) {
        fprintf(file, "%s\n", current->text);
        current = current->next;
    }

    fclose(file);
    printf("Text was saved successfully.\n");
}

void load_text_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char buffer[100];// масив
    while(fgets(buffer, 100, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // рахуємо кількість символів до /n і змінюємо на /0
        Line *new_line = (Line *)malloc(sizeof(Line));
        new_line->text = strdup(buffer);//текст з буферу копіюється в text
        new_line->next = NULL;
        if(head == NULL) {
            head = new_line;
        }
        else {
            Line *current = head;
            while(current->next != NULL) {
                current = current->next;
            }
            current->next = new_line;
        }
    }
    fclose(file);
    printf("Text loaded successfully.\n");
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

void find_longest_substring() {
    char substring[100];
    printf("Enter the substring to search for: ");
    fgets(substring, 100, stdin);
    substring[strcspn(substring, "\n")] = '\0';
    Line *current = head;
    int line_number = 0;
    int found = 0;

    while (current != NULL) {
        char *text = current->text;
        char *match = strstr(text, substring);//повертає індекс match

        while (match != NULL) {
            found = 1;
            int index = match - text; //різниця між адресами
            printf("Match found at line %d, index %d\n", line_number, index);
            match = strstr(match + 1, substring); // Продовжуємо шукати після першого знайденого
        }

        current = current->next;
        line_number++;
    }

    if (!found) {
        printf("No matches found.\n");
    }
}

void insert_substring() {
    int line, index;
    char insert_text[100];

    printf("Choose line and index to insert the substring (line number index number): ");
    scanf("%d %d", &line, &index);
    getchar();
    printf("Enter the substring to insert: ");
    fgets(insert_text, 100, stdin);
    insert_text[strcspn(insert_text, "\n")] = '\0';
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

    strcat(new_text, &current->text[index]);
    //звільняємо старий текст записуємо новий
    free(current->text);
    current->text = new_text;

    printf("Substring inserted successfully.\n");
}

void delete_all_text() {
    free_memory();
    head = NULL;
    printf("All text has been cleared.\n");
}

void clean_console() {
    system("cls");
}

void print_help() {
    printf("Commands:\n");
    printf("0: Exit\n");
    printf("1: Write your text\n");
    printf("2: Start new line\n");
    printf("3: Print your current text\n");
    printf("4: Save text to file: saved_text\n");
    printf("5: Load text from file\n");
    printf("6: Insert text\n");
    printf("7: Find longest substring\n");
    printf("8: Insert_substring\n");
    printf("9: Help\n");
    printf("10: Delete all text\n");
    printf("11: Clean console\n");
}
