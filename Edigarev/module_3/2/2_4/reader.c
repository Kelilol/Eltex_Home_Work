#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    // Создаем именованный канал (FIFO)
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
    }

    // Открываем FIFO для чтения
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Чтение случайных чисел из FIFO
    int num;
    while (read(fd, &num, sizeof(int)) > 0) {
        printf("Received number: %d\n", num);
    }

    // Закрываем FIFO
    close(fd);
    return 0;
}
