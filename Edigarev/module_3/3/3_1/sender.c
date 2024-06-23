#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_MSG_SIZE 256

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int main() {
    int msqid;
    key_t key;
    struct msg_buffer message;

    // Генерируем уникальный ключ для очереди сообщений
    key = ftok("msgqfile", 'B');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Создаем очередь сообщений или подключаемся к существующей
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }

    // Отправляем сообщения в очередь
    printf("Отправка сообщений в очередь (System V):\n");
    printf("Введите сообщение ('exit' для завершения):\n");

    while (1) {
        fgets(message.msg_text, MAX_MSG_SIZE, stdin);

        // Удаляем символ новой строки в конце сообщения
        size_t len = strlen(message.msg_text);
        if (len > 0 && message.msg_text[len - 1] == '\n') {
            message.msg_text[len - 1] = '\0';
        }

        // Определяем тип сообщения
        message.msg_type = 1;

        // Отправляем сообщение в очередь
        if (msgsnd(msqid, &message, MAX_MSG_SIZE, 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        // Проверка на завершение
        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }
    }

    // Удаляем очередь сообщений
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    printf("Очередь сообщений удалена\n");

    return 0;
}
