#define _POSIX_C_SOURCE 200809L  // Пример определения POSIX версии, которую вы хотите использовать
#define _GNU_SOURCE  // Если вам нужны расширенные функции GNU
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

volatile sig_atomic_t file_locked = 0;

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
    FILE *file = fopen("data.txt", "w");
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
            // Open file for reading
            file_descriptor = open("data.txt", O_RDONLY);
            if (file_descriptor == -1) {
                perror("open");
                return 1;
            }

            // Check if file access is locked
            while (file_locked) {
                sleep(1); // Wait until file access is unlocked
            }

            // Read from file
            if (read(file_descriptor, buffer, sizeof(buffer)) == -1) {
                perror("read");
                return 1;
            }

            // Close file
            close(file_descriptor);

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
            file_descriptor = open("data.txt", O_WRONLY | O_TRUNC);
            if (file_descriptor == -1) {
                perror("open");
                return 1;
            }

            // Write new random number to file
            random_number = rand() % 100;
            dprintf(file_descriptor, "%d\n", random_number);

            // Close file
            close(file_descriptor);

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
