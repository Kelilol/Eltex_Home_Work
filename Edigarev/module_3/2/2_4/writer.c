#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    // Создаем именованный канал (FIFO)
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
    }

    // Открываем FIFO для записи
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация и запись случайных чисел в FIFO
    for (int i = 0; i < 10; ++i) {
        int num = rand() % 100;
        if (write(fd, &num, sizeof(int)) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        sleep(1); // Задержка для демонстрации
    }

    // Закрываем FIFO
    close(fd);
    return 0;
}
