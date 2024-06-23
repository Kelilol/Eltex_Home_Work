#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void generate_random_numbers(int count, int pipe_fd1[2], int pipe_fd2[2]) {
    close(pipe_fd1[0]); // Закрываем чтение со стороны потомка для pipe1
    close(pipe_fd2[1]); // Закрываем запись со стороны потомка для pipe2

    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int num = rand() % 100;
        write(pipe_fd1[1], &num, sizeof(int)); // Отправляем случайное число родителю

        int response;
        read(pipe_fd2[0], &response, sizeof(int)); // Читаем ответ от родителя
        printf("Child received response: %d\n", response);
    }

    close(pipe_fd1[1]); // Закрываем запись после генерации всех чисел
    close(pipe_fd2[0]); // Закрываем чтение после получения всех ответов
    exit(EXIT_SUCCESS);
}

void write_numbers_to_file_and_respond(int count, int pipe_fd1[2], int pipe_fd2[2]) {
    close(pipe_fd1[1]); // Закрываем запись со стороны родителя для pipe1
    close(pipe_fd2[0]); // Закрываем чтение со стороны родителя для pipe2

    FILE *file = fopen("random_numbers.txt", "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        int num;
        read(pipe_fd1[0], &num, sizeof(int)); // Читаем случайное число от потомка
        printf("Parent received: %d\n", num);
        fprintf(file, "%d\n", num);

        int response = num * 2;
        write(pipe_fd2[1], &response, sizeof(int)); // Отправляем ответ потомку
    }

    close(pipe_fd1[0]); // Закрываем чтение после получения всех чисел
    close(pipe_fd2[1]); // Закрываем запись после отправки всех ответов
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_random_numbers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);
    if (count <= 0) {
        fprintf(stderr, "Number of random numbers must be greater than 0\n");
        exit(EXIT_FAILURE);
    }

    int pipe_fd1[2]; // Первый канал для передачи данных от потомка к родителю
    int pipe_fd2[2]; // Второй канал для передачи данных от родителя к потомку

    if (pipe(pipe_fd1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe_fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        generate_random_numbers(count, pipe_fd1, pipe_fd2);
    } else {
        write_numbers_to_file_and_respond(count, pipe_fd1, pipe_fd2);
        wait(NULL); // Ожидание завершения потомка
    }

    return 0;
}
