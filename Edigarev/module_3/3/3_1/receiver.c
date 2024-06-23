#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

    // Получаем доступ к существующей очереди сообщений
    msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Прием сообщений из очереди (System V):\n");

    // Получаем сообщения из очереди
    while (1) {
        // Получаем любое сообщение из очереди
        if (msgrcv(msqid, &message, MAX_MSG_SIZE, 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        // Проверка на завершение
        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }

        // Выводим сообщение на экран
        printf("Received: %s\n", message.msg_text);
    }

    return 0;
}
