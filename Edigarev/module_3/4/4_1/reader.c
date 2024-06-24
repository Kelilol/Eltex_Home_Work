#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_KEY 1234

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int semid;

// Функция для получения семафора
int get_semaphore() {
    int semid = semget(SEM_KEY, 1, 0);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    return semid;
}

// Функция для блокировки семафора
void semaphore_lock() {
    struct sembuf sem_lock = {0, -1, SEM_UNDO};
    if (semop(semid, &sem_lock, 1) == -1) {
        perror("semop lock");
        exit(EXIT_FAILURE);
    }
}

// Функция для разблокировки семафора
void semaphore_unlock() {
    struct sembuf sem_unlock = {0, 1, SEM_UNDO};
    if (semop(semid, &sem_unlock, 1) == -1) {
        perror("semop unlock");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Создаем и открываем FIFO для чтения
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Получаем идентификатор семафора
    semid = get_semaphore();

    // Чтение и вывод случайных чисел из FIFO
    int num;
    while (read(fd, &num, sizeof(int)) > 0) {
        semaphore_lock();

        printf("Принято число: %d\n", num);

        semaphore_unlock();
    }

    // Закрываем FIFO
    close(fd);

    return 0;
}
