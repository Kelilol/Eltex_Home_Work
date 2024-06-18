//permissions.c
#include "permissions.h"
void display_permissions(mode_t mode) {
    printf("Битовое представление прав доступа: %d\n", mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

// Функция для отображения буквенного и цифрового представления прав доступа
void display_permissions_info(mode_t mode) {
    printf("Буквенное представление прав доступа: ");
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("Цифровое представление прав доступа: %o\n", mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

// Функция для преобразования строкового представления прав доступа в битовое
mode_t parse_permissions(const char *perms) {
    mode_t mode = 0;

    if (perms[0] >= '0' && perms[0] <= '7') {
        mode = (mode_t)strtol(perms, NULL, 8);
    } else {
        for (int i = 0; i < 9; i++) {
            switch (perms[i]) {
                case 'r':
                    mode |= (/*((i % 3) == 0) &&*/ ((i / 3) == 0)) ? S_IRUSR : (/*((i % 3) == 0) &&*/ ((i / 3) == 1)) ? S_IRGRP : (/*((i % 3) == 0) &&*/ ((i / 3) == 2)) ? S_IROTH : 0;
                break;
                case 'w':
                    mode |= (/*((i % 3) == 1) &&*/ ((i / 3) == 0)) ? S_IWUSR : (/*((i % 3) == 1) &&*/ ((i / 3) == 1)) ? S_IWGRP : (/*((i % 3) == 1) && */((i / 3) == 2)) ? S_IWOTH : 0;
                break;
                case 'x':
                    mode |= (/*((i % 3) == 2) && */((i / 3) == 0)) ? S_IXUSR : (/*((i % 3) == 2) &&*/ ((i / 3) == 1)) ? S_IXGRP : (/*((i % 3) == 2) &&*/ ((i / 3) == 2)) ? S_IXOTH : 0;
                break;
                default:
                // Если символ не 'r', 'w' или 'x', пропустить его
                break;
            }
        }
    }

    return mode;
}

mode_t parse_mode(const char *mode_str) {
    mode_t mode = 0;
    const char *ptr = mode_str;

    while (*ptr) {
        mode_t part = 0;
        int read = 0, write = 0, exec = 0;

        while (*ptr && *ptr != '=') {
            switch (*ptr) {
                case 'u': read = S_IRUSR; write = S_IWUSR; exec = S_IXUSR; break;
                case 'g': read = S_IRGRP; write = S_IWGRP; exec = S_IXGRP; break;
                case 'o': read = S_IROTH; write = S_IWOTH; exec = S_IXOTH; break;
            }
            ptr++;
        }

        if (*ptr == '=') ptr++;

        while (*ptr && *ptr != ',') {
            switch (*ptr) {
                case 'r': part |= read; break;
                case 'w': part |= write; break;
                case 'x': part |= exec; break;
            }
            ptr++;
        }

        mode |= part;

        if (*ptr == ',') ptr++;
    }

    return mode;
}
