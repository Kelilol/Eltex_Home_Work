#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_NAME "/my_semaphore"

sem_t *sem;

int main() {
    int fd;
    int num;

    // Создаем или открываем FIFO для записи
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    fd = open(FIFO_PATH, O_WRONLY);
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

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    // Генерируем и записываем случайные числа в FIFO
    for (int i = 0; i < 10; ++i) {
        sem_wait(sem); // Блокируем доступ к критической секции

        num = rand() % 100;
        if (write(fd, &num, sizeof(int)) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        printf("Отправлено число: %d\n", num);

        sem_post(sem); // Разблокируем доступ к критической секции

        sleep(1); // Задержка для демонстрации
    }

    // Закрываем и удаляем семафор POSIX
    sem_close(sem);
    sem_unlink(SEM_NAME);

    // Закрываем и удаляем FIFO
    close(fd);
    unlink(FIFO_PATH);

    return 0;
}
