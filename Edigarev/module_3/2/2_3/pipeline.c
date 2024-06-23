#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 10
#define MAX_ARGS 100

void execute_pipeline(char *commands[], int num_commands) {
    int i;
    int pipefd[2*num_commands];

    // Создаем пайпы
    for (i = 0; i < num_commands; i++) {
        if (pipe(pipefd + i*2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int j = 0;
    for (i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Настройка каналов ввода и вывода для дочернего процесса
            if (i != 0) {
                if (dup2(pipefd[(i-1)*2], 0) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            if (i != num_commands - 1) {
                if (dup2(pipefd[i*2+1], 1) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Закрываем все пайпы в дочернем процессе
            for (j = 0; j < 2*num_commands; j++) {
                close(pipefd[j]);
            }

            // Разбираем команду и её аргументы
            char *args[MAX_ARGS];
            char *token = strtok(commands[i], " ");
            int k = 0;
            while (token != NULL) {
                args[k] = token;
                token = strtok(NULL, " ");
                k++;
            }
            args[k] = NULL;

            // Выполняем команду
            if (execvp(args[0], args) < 0) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Закрываем все пайпы в родительском процессе
    for (i = 0; i < 2*num_commands; i++) {
        close(pipefd[i]);
    }

    // Ждем завершения всех дочерних процессов
    for (i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <команда1> [<команда2> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    execute_pipeline(&argv[1], argc - 1);

    return EXIT_SUCCESS;
}
