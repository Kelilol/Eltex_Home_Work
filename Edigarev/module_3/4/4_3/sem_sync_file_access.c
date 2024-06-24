#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SEM_KEY 0x1234
#define FILENAME "data.txt"

volatile sig_atomic_t file_locked = 0;
int sem_id;

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

int main() {
    pid_t pid;
    int random_number;
    char buffer[1024];
    struct sigaction sa_usr1, sa_usr2;
    struct sembuf sem_lock = {0, -1, SEM_UNDO}; // Семафор блокировки
    struct sembuf sem_unlock = {0, 1, SEM_UNDO}; // Семафор разблокировки

    // Создаем или получаем идентификатор семафора
    sem_id = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id == -1) {
        sem_id = semget(SEM_KEY, 1, 0);
    } else {
        // Инициализируем семафор
        if (semctl(sem_id, 0, SETVAL, 1) == -1) {
            perror("semctl");
            return 1;
        }
    }

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
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        perror("fopen");
        return 1;
    }
    fprintf(file, "%d\n", 0);
    fclose(file);

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        while (1) {
            // Wait until file access is unlocked
            semop(sem_id, &sem_lock, 1);

            // Open file for reading
            file = fopen(FILENAME, "r");
            if (!file) {
                perror("fopen");
                semop(sem_id, &sem_unlock, 1);
                return 1;
            }

            // Read from file
            if (fgets(buffer, sizeof(buffer), file) == NULL) {
                perror("fgets");
                semop(sem_id, &sem_unlock, 1);
                return 1;
            }
            fclose(file);

            // Convert read data to integer
            random_number = atoi(buffer);

            // Print the read number
            printf("Child process read: %d\n", random_number);

            // Send random number to parent process
            kill(getppid(), SIGUSR1);

            // Release the lock
            semop(sem_id, &sem_unlock, 1);
            sleep(1); // Sleep for demonstration purpose
        }
    } else {
        // Parent process
        int counter = 1;
        while (1) {
            // Wait until file access is unlocked
            semop(sem_id, &sem_lock, 1);

            // Modify the file data.txt
            file = fopen(FILENAME, "w");
            if (!file) {
                perror("fopen");
                semop(sem_id, &sem_unlock, 1);
                return 1;
            }

            // Write new random number to file
            random_number = rand() % 100;
            fprintf(file, "%d\n", random_number);
            fclose(file);

            // Notify child process that file modification is complete
            kill(pid, SIGUSR2);
            printf("Parent process modified file: %d\n", random_number);
            counter++;

            // Release the lock
            semop(sem_id, &sem_unlock, 1);
            sleep(2); // Sleep for demonstration purpose
        }
    }

    // Удаляем семафор
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}
