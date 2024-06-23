#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void generate_random_numbers(int count, int write_fd) {
    srand(time(NULL));
    for (int i = 0; i < count; ++i) {
        int num = rand() % 100; // Генерируем случайное число от 0 до 99
        printf("Num Childe %d\n", num);
	write(write_fd, &num, sizeof(num));
    }
}

void write_numbers_to_file_and_screen(int count, int read_fd) {
    FILE *file = fopen("numbers.txt", "w");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; ++i) {
        int num;
        read(read_fd, &num, sizeof(num));
        printf("Number: %d\n", num);
        fprintf(file, "Number: %d\n", num);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of random numbers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(stderr, "The number of random numbers must be greater than 0\n");
        return EXIT_FAILURE;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // Дочерний процесс
        close(pipefd[0]); // Закрываем неиспользуемый конец для чтения
        generate_random_numbers(count, pipefd[1]);
        close(pipefd[1]); // Закрываем конец для записи после отправки данных
    } else {
        // Родительский процесс
        close(pipefd[1]); // Закрываем неиспользуемый конец для записи
        write_numbers_to_file_and_screen(count, pipefd[0]);
        close(pipefd[0]); // Закрываем конец для чтения после получения данных
    }

    return EXIT_SUCCESS;
}

