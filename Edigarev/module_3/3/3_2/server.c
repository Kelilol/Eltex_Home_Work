#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 100

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
};

int main() {
    key_t key;
    int msqid;
    struct msgbuf message;

    // Генерируем уникальный ключ
    key = ftok("msgqfile", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Получаем доступ к очереди сообщений
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидание сообщений...\n");

    // Получаем сообщения от клиентов
    while (1) {
        // Получаем любое сообщение из очереди
        if (msgrcv(msqid, &message, sizeof(message.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Получено сообщение от клиента с типом %ld: %s\n", message.mtype, message.mtext);

        // Отправляем ответ клиенту
        snprintf(message.mtext, MSG_SIZE, "Принято сообщение от клиента с типом %ld", message.mtype);
        message.mtype = message.mtype; // Отправляем ответ клиенту с тем же типом
        if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
