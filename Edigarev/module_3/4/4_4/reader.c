#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_NAME "/my_semaphore"

sem_t *sem;

int main() {
    int fd;
    int num;

    // Открываем FIFO для чтения
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Открываем или создаем семафор POSIX
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Читаем и выводим данные из FIFO
    while (read(fd, &num, sizeof(int)) > 0) {
        sem_wait(sem); // Блокируем доступ к критической секции

        printf("Принято число: %d\n", num);

        sem_post(sem); // Разблокируем доступ к критической секции
    }

    // Закрываем и удаляем семафор POSIX
    sem_close(sem);
    sem_unlink(SEM_NAME);

    // Закрываем FIFO
    close(fd);

    return 0;
}
