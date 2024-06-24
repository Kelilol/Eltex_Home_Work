#define _XOPEN_SOURCE 700  // Устанавливаем POSIX версию
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEM_KEY 1234
#define FILE_PATH "data.txt"

volatile sig_atomic_t file_locked = 0;
int semid;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void handle_sigusr1(int sig) {
    if (sig == SIGUSR1) {
        file_locked = 1;
        printf("File access locked.\n");
    }
}

void handle_sigusr2(int sig) {
    if (sig == SIGUSR2) {
        file_locked = 0;
        printf("File access unlocked.\n");
    }
}

int create_or_get_semaphore() {
    int semid;
    union semun arg;
    struct sembuf sem_lock = {0, -1, SEM_UNDO};   // Уменьшение семафора
    struct sembuf sem_unlock = {0, 1, SEM_UNDO};  // Увеличение семафора

    semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666); // Создание нового семафора
    if (semid == -1) {
        if (errno == EEXIST) {
            semid = semget(SEM_KEY, 1, 0); // Получение существующего семафора
            printf("Semaphore already exists, getting its identifier: %d\n", semid);
        } else {
            perror("semget");
            exit(EXIT_FAILURE);
        }
    } else {
        arg.val = 1; // Устанавливаем начальное значение семафора (1 - разблокирован)
        if (semctl(semid, 0, SETVAL, arg) == -1) { // Устанавливаем значение семафора
            perror("semctl");
            exit(EXIT_FAILURE);
        }
        printf("Created new semaphore with identifier: %d\n", semid);
    }

    return semid;
}

void semaphore_lock() {
    struct sembuf sem_lock = {0, -1, SEM_UNDO};
    if (semop(semid, &sem_lock, 1) == -1) { // Уменьшение семафора
        perror("semop lock");
        exit(EXIT_FAILURE);
    }
}

void semaphore_unlock() {
    struct sembuf sem_unlock = {0, 1, SEM_UNDO};
    if (semop(semid, &sem_unlock, 1) == -1) { // Увеличение семафора
        perror("semop unlock");
        exit(EXIT_FAILURE);
    }
}

int main() {
    pid_t pid;
    int file_descriptor;
    int random_number;
    char buffer[1024];
    struct sigaction sa_usr1, sa_usr2;

    // Setup SIGUSR1 handler
    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    // Setup SIGUSR2 handler
    sa_usr2.sa_handler = handle_sigusr2;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;
    sigaction(SIGUSR2, &sa_usr2, NULL);

    // Create a file "data.txt" with initial content
    FILE *file = fopen(FILE_PATH, "w");
    if (!file) {
        perror("fopen");
        return 1;
    }
    fprintf(file, "%d\n", 0);
    fclose(file);

    // Create or get semaphore
    semid = create_or_get_semaphore();

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        while (1) {
            // Open file for reading
            semaphore_lock();  // Блокировка доступа к файлу
            file_descriptor = open(FILE_PATH, O_RDONLY);
            if (file_descriptor == -1) {
                perror("open");
                return 1;
            }

            // Read from file
            if (read(file_descriptor, buffer, sizeof(buffer)) == -1) {
                perror("read");
                return 1;
            }

            // Close file
            close(file_descriptor);
            semaphore_unlock(); // Разблокировка доступа к файлу

            // Convert read data to integer
            random_number = atoi(buffer);

            // Print the read number
            printf("Child process read: %d\n", random_number);

            // Send random number to parent process
            kill(getppid(), SIGUSR1);
            sleep(1); // Sleep for demonstration purpose
        }
    } else {
        // Parent process
        int counter = 1;
        while (1) {
            // Modify the file data.txt
            semaphore_lock();  // Блокировка доступа к файлу
            file_descriptor = open(FILE_PATH, O_WRONLY | O_TRUNC);
            if (file_descriptor == -1) {
                perror("open");
                return 1;
            }

            // Write new random number to file
            random_number = rand() % 100;
            dprintf(file_descriptor, "%d\n", random_number);

            // Close file
            close(file_descriptor);
            semaphore_unlock(); // Разблокировка доступа к файлу

            // Notify child process that file modification is complete
            kill(pid, SIGUSR2);
            printf("Parent process modified file: %d\n", random_number);
            counter++;

            // Sleep for demonstration purpose
            sleep(2);
        }
    }

    return 0;
}
