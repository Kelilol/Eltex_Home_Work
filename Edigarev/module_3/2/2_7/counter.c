#define _POSIX_C_SOURCE 200809L  // Пример определения POSIX версии, которую вы хотите использовать
#define _GNU_SOURCE  // Если вам нужны расширенные функции GNU
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigint_count = 0;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        sigint_count++;
        printf("Пойман SIGINT (%d)\n", sigint_count);
        if (sigint_count >= 3) {
            printf("Получено SIGINT три раза, выход из программы.\n");
            exit(0);
        }
    } else if (sig == SIGQUIT) {
        printf("Пойман SIGQUIT\n");
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    FILE *file = fopen("counter.txt", "w");
    if (!file) {
        perror("fopen");
        return 1;
    }

    int counter = 0;
    while (1) {
        counter++;
        fprintf(file, "%d\n", counter);
        fflush(file);
        sleep(1);
    }

    fclose(file);
    return 0;
}