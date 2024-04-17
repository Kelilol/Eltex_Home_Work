//main.c
#include "permissions.h"

void cleaninput(char *input){// Проверяем переполнение буфера
    if (strchr(input, '\n') == NULL) {
        // Очищаем остаток буфера
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        // Удаляем символ новой строки, если он присутствует
        input[strcspn(input, "\n")] = '\0';
    }
}

void printMenu() {
    printf("\nMenu:\n");
    printf("1. Ввеcти права доступа в буквенном или цифровом обозначении:\n");
    printf("2. Введите имя файла для сканироварования прав доступа файла: \n");
    printf("3. Введти командe модификации атрибутов (подобно cmode): \n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}


int main() {
    char permissions_str[10],mode_str[17];
    char filename[100];
    mode_t mode;
    struct stat file_stat;
    char choice[2];
    int choice_mode;
    do {
        printMenu();
        //scanf("%d", &choice);
        fgets(choice, 2, stdin);
        cleaninput(choice);
        choice_mode=atoi(choice);
        switch (choice_mode) {
            case 1:
                    printf("Введите права доступа в буквенном или цифровом обозначении: ");
                    fgets(permissions_str, 10, stdin);
                    cleaninput(permissions_str);
                    mode = parse_permissions(permissions_str);
                     display_permissions(mode);
                    // Отображение буквенного и цифрового представления
                    display_permissions_info(mode);
                break;
            case 2:
                    printf("Введите имя файла: ");
                    fgets(filename, 100, stdin);
                    cleaninput(filename);
                        if (stat(filename, &file_stat) != -1) {
                            printf("Сравнение с выводом ls -l:\n");
                            printf("Битовое представление: %o\n", file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
                            display_permissions_info(file_stat.st_mode);
                        } else {
                            printf("Ошибка: не удалось получить информацию о файле\n");
                            //return EXIT_FAILURE;
                        }

                break;
            case 3:
                    printf("Введите команды модификации атрибутов (подобно u=rw,g=xw,o=r): ");
                 
                    fgets(mode_str, 17, stdin);
                    cleaninput(mode_str);

                    // Отображение измененных прав доступа
                    mode = parse_mode(mode_str);
                    display_permissions(mode);
                    display_permissions_info(mode);
                break;
            case 4:
                
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice_mode != 4);

    return 0;
}


