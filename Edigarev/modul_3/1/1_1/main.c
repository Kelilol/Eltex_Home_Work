#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void on_exit_function() {
    printf("Program has exited normally.\n");
}

int main(int argc, char *argv[]) {
    // Register the exit function
    if (atexit(on_exit_function) != 0) {
        perror("atexit");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // Child process
        printf("Child process:\n");
        printf("PID: %d\n", getpid());
        printf("Arguments:\n");
        for (int i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, argv[i]);
        }
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent process:\n");
        printf("PID: %d\n", getpid());
        printf("Arguments:\n");
        for (int i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, argv[i]);
        }

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);
    }

    return EXIT_SUCCESS;
}
