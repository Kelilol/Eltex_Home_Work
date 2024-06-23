#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigint_count = 0;

void handle_sigint(int sig) {
    sigint_count++;
    if (sigint_count >= 3) {
        printf("Caught SIGINT (%d), exiting now.\n", sigint_count);
        exit(0);
    } else {
        printf("Caught SIGINT (%d)\n", sigint_count);
    }
}

int main() {
    signal(SIGINT, handle_sigint);

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
