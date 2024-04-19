#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10

void child_process(int pipe_fsd[], int num_numbers) {
    close(pipe_fsd[0]); // Закрываем неиспользуемый конец канала

    for (int i = 0; i < num_numbers; i++) {
        int number = rand() % 100 + 20; // Генерируем случайное число от 1 до 100
        write(pipe_fsd[1], &number, sizeof(number)); // Отправляем число родительскому процессу
        //printf("%d дочка %d\n", i,number);
        //sleep(2);
    }

    close(pipe_fsd[1]); // Закрываем конец канала после отправки всех чисел
}

void parent_process(int pipe_fds[], int num_numbers) {
    close(pipe_fds[1]); // Закрываем неиспользуемый конец канала

    FILE *file = fopen("numbers.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Received numbers:\n");
    for (int i = 0; i < num_numbers; i++) {
        int number;
        read(pipe_fds[0], &number, sizeof(number)); // Получаем число от дочернего процесса
        //sleep(2);
        //printf("%d родитель %d\n", i,number);
        printf("%d\n", number);
        fprintf(file, "%d\n", number); // Записываем число в файл
    }

    fclose(file);
    close(pipe_fds[0]); // Закрываем конец канала после получения всех чисел
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of numbers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_numbers = atoi(argv[1]);

    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Дочерний процесс
        child_process(pipe_fds, num_numbers);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс
        parent_process(pipe_fds, num_numbers);
        wait(NULL); // Ожидаем завершения дочернего процесса
        exit(EXIT_SUCCESS);
    }
}
