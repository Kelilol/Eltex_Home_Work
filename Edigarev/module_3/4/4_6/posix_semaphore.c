#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>

#define READ_SEM_NAME "/read_semaphore"
#define WRITE_SEM_NAME "/write_semaphore"
#define FILENAME "data.txt"
#define MAX_READERS 5  // Максимальное количество процессов, которые могут читать одновременно
#define NUM_CHILDREN 3 // Количество дочерних процессов

volatile sig_atomic_t file_locked = 0;
sem_t *read_sem;
sem_t *write_sem;

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

    // Create or open the POSIX semaphores
    read_sem = sem_open(READ_SEM_NAME, O_CREAT | O_EXCL, 0666, MAX_READERS);
    if (read_sem == SEM_FAILED) {
        if (errno == EEXIST) {
            read_sem = sem_open(READ_SEM_NAME, 0);  // Semaphore already exists
        } else {
            perror("sem_open read");
            return 1;
        }
    }

    write_sem = sem_open(WRITE_SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (write_sem == SEM_FAILED) {
        if (errno == EEXIST) {
            write_sem = sem_open(WRITE_SEM_NAME, 0);  // Semaphore already exists
        } else {
            perror("sem_open write");
            return 1;
        }
    }

    // Fork NUM_CHILDREN child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Child process
            while (1) {
                // Wait until file access is unlocked
                sem_wait(read_sem);  // Decrement the read semaphore
                sem_wait(write_sem); // Ensure no writers are active

                // Open file for reading
                file = fopen(FILENAME, "r");
                if (!file) {
                    perror("fopen");
                    sem_post(read_sem);
                    sem_post(write_sem);
                    return 1;
                }

                // Read from file
                if (fgets(buffer, sizeof(buffer), file) == NULL) {
                    perror("fgets");
                    sem_post(read_sem);
                    sem_post(write_sem);
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
                sem_post(write_sem); // Allow writers to proceed
                sem_post(read_sem);  // Increment the read semaphore
                sleep(1); // Sleep for demonstration purpose
            }
            exit(0); // Важно завершить дочерний процесс
        }
    }

    // Parent process
    while (1) {
        // Wait until file access is unlocked
        sem_wait(write_sem); // Ensure no readers or writers are active

        // Modify the file data.txt
        file = fopen(FILENAME, "w");
        if (!file) {
            perror("fopen");
            sem_post(write_sem);
            return 1;
        }

        // Write new random number to file
        random_number = rand() % 100;
        fprintf(file, "%d\n", random_number);
        fclose(file);

        // Notify child process that file modification is complete
        kill(pid, SIGUSR2);
        printf("Parent process modified file: %d\n", random_number);

        // Release the lock
        sem_post(write_sem); // Allow readers to proceed
        sleep(2); // Sleep for demonstration purpose
    }

    // Close and unlink the semaphores
    sem_close(read_sem);
    sem_unlink(READ_SEM_NAME);
    sem_close(write_sem);
    sem_unlink(WRITE_SEM_NAME);

    return 0;
}
