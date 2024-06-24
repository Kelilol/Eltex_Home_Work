#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h> // Для работы с семафорами
#include <errno.h>   // Для работы с ошибками

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_KEY 1234

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int semid;

// Функция для создания или получения семафора
int create_or_get_semaphore() {
    int semid;
    union semun arg;

    semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666); // Создание нового семафора
    if (semid == -1) {
        if (errno == EEXIST) {
            semid = semget(SEM_KEY, 1, 0); // Получение существующего семафора
            printf("Семафор уже существует, получаем его идентификатор: %d\n", semid);
        } else {
            perror("semget");
            exit(EXIT_FAILURE);
        }
    } else {
        arg.val = 1; // Устанавливаем начальное значение семафора
        if (semctl(semid, 0, SETVAL, arg) == -1) { // Устанавливаем значение семафора
            perror("semctl");
            exit(EXIT_FAILURE);
        }
        printf("Создан новый семафор с идентификатором: %d\n", semid);
    }

    return semid;
}

// Функция для блокировки семафора
void semaphore_lock() {
    struct sembuf sem_lock = {0, -1, SEM_UNDO};
    if (semop(semid, &sem_lock, 1) == -1) { // Блокировка семафора
        perror("semop lock");
        exit(EXIT_FAILURE);
    }
}

// Функция для разблокировки семафора
void semaphore_unlock() {
    struct sembuf sem_unlock = {0, 1, SEM_UNDO};
    if (semop(semid, &sem_unlock, 1) == -1) { // Разблокировка семафора
        perror("semop unlock");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Создаем и открываем FIFO для записи
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Получаем идентификатор семафора
    semid = create_or_get_semaphore();

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация и запись случайных чисел в FIFO
    for (int i = 0; i < 10; ++i) {
        semaphore_lock();

        int num = rand() % 100;
        if (write(fd, &num, sizeof(int)) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        printf("Отправлено число: %d\n", num);

        semaphore_unlock();

        sleep(1); // Задержка для демонстрации
    }

    // Закрываем и удаляем FIFO
    close(fd);
    unlink(FIFO_PATH);

    return 0;
}
