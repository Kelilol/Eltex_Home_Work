#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

void prompt() {
    printf("my_shell> ");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;

    while (1) {
        prompt();
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            break;
        }

        // Remove newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input
        int i = 0;
        token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue; // No command entered
        }

        if (strcmp(args[0], "exit") == 0) {
            break; // Exit the shell
        }

        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process: execute the command
            execvp(args[0], args);
            // If execvp returns, there was an error
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // Parent process: wait for the child to complete
            waitpid(pid, &status, 0);
        }
    }

    return EXIT_SUCCESS;
}
