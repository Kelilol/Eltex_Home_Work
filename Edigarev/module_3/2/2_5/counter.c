#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
    if (sig == SIGTERM || sig == SIGINT || sig == SIGQUIT || sig == SIGABRT) {
        stop = 1;
    }
}

int main() {
    FILE *file = fopen("counter.txt", "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGABRT, handle_signal);

    int counter = 0;
    while (!stop) {
        fprintf(file, "%d\n", counter++);
        fflush(file);
        sleep(1);
    }

    fclose(file);
    return 0;
}
