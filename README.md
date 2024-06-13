# Assignment 1: Simple Text Editor :page_with_curl:
## Опис :black_nib:

Ця програма є простим текстовим редактором, реалізованим на мові C. Вона дозволяє користувачам виконувати різні операції з текстом:

- **exit** (вихід з програми);
- **write_text** (запис тексту);
- **start_new_line** (почати з нової лінійки);
- **print_text** (вивід поточного тексту);
- **save_text_to_file** (збереження тексту у файл);
- **load_text_from_file** (вивантаження тексту з файлу);
- **insert_text** (вставка тексту);
- **find_longest_substring** (найти найдовший підрядок);
- **insert_substring** (змінити підрядок);
- **print_help** (інформація про всі функції);
- **delete_all_text** (видалення тексту);
- **clean_console** (очищення console);

![commands](https://github.com/vloziak/Simple-text-editor/blob/main/Commands.png?raw=true,"commands")

-----
## Example Usage :smiley:

### Example 1:

1. Write text;
2. Start new line;
3. Find substring;
4. Insert substring;
5. Print text;
6. Save text to file;

![example1](https://github.com/vloziak/Simple-text-editor/blob/main/Example1.png?raw=true,"example1")

![saved_text](https://github.com/vloziak/Simple-text-editor/blob/main/savedtext.png?raw=true,"saved_text")

--------------

## Example 2:

1. Load from file;
2. Print text;
3. Write text;
4. Print text;

![example2.1](https://github.com/vloziak/Simple-text-editor/blob/main/Example2.png?raw=true, "example2.1")
![example2.2](https://github.com/vloziak/Simple-text-editor/blob/main/Example2.2.png?raw=true, "example2.2")

-----------------

# Assignment 2: Simple Text Editor: the basics of object oriented programming
## Опис 2 практичної:

У другому завданні ми розширили простий текстовий редактор, використовуючи принципи об'єктно-орієнтованого програмування (ООП) на мові C++.

## Додані функції:

- **insert_text** (доданий режим replacement);
- **undo** (повернення до попереднього стану тексту);
- **redo** (відновлення стану після undo);
- **cut** ()