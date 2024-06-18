#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Функция для вычисления площади квадрата
double calculate_square_area(double side_length) {
    return side_length * side_length;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <side_length1> <side_length2> ... <side_lengthN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_squares = argc - 1;  // Количество сторон (аргументов)
    int half = (num_squares + 1) / 2;  // Половина количества сторон для распределения между процессами

    pid_t pid = fork();

    if (pid < 0) {
        // Ошибка при вызове fork
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Дочерний процесс
        printf("Child process calculating squares:\n");
        for (int i = 1; i <= half; i++) {
            double side_length = atof(argv[i]);
            double area = calculate_square_area(side_length);
            printf("Side length: %f, Area: %f\n", side_length, area);
        }
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс
        printf("Parent process calculating squares:\n");
        for (int i = half + 1; i < argc; i++) {
            double side_length = atof(argv[i]);
            double area = calculate_square_area(side_length);
            printf("Side length: %f, Area: %f\n", side_length, area);
        }

        // Ожидание завершения дочернего процесса
        int status;
        waitpid(pid, &status, 0);
    }

    return EXIT_SUCCESS;
}
