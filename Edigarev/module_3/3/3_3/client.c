#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 256

struct msgbuf {
    long mtype; // тип сообщения (ID клиента)
    char mtext[MSG_SIZE];
};

int main() {
    key_t key = ftok("msgqfile", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Получаем доступ к очереди сообщений
    int msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Генерируем ID клиента (тип сообщения)
    long client_id = getpid();

    printf("Клиент запущен. ID клиента: %ld\n", client_id);

    struct msgbuf message;
    message.mtype = client_id;

    while (1) {
        printf("Введите сообщение: ");
        fgets(message.mtext, sizeof(message.mtext), stdin);

        // Убираем символ новой строки, который добавляется функцией fgets
        message.mtext[strcspn(message.mtext, "\n")] = '\0';

        // Отправляем сообщение серверу
        if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        // Ожидаем ответа от сервера
        if (msgrcv(msqid, &message, sizeof(message.mtext), client_id, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Выводим полученное сообщение
        printf("Ответ от сервера: %s\n", message.mtext);
    }

    return 0;
}
