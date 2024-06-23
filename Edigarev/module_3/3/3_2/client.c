#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h> // Для функции getpid()

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

    // Определяем тип клиента как его PID
    message.mtype = getpid();

    // Отправляем сообщения серверу
    while (1) {
        printf("Введите сообщение: ");
        fgets(message.mtext, MSG_SIZE, stdin);

        // Убираем символ новой строки, который добавляет fgets
        message.mtext[strcspn(message.mtext, "\n")] = '\0';

        // Отправляем сообщение серверу
        if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        // Получаем ответ от сервера
        if (msgrcv(msqid, &message, sizeof(message.mtext), getpid(), 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Получен ответ от сервера: %s\n", message.mtext);

        // Выход из цикла при вводе "exit"
        if (strncmp(message.mtext, "exit", 4) == 0)
            break;
    }

    return 0;
}
