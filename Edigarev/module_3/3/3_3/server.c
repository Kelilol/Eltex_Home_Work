#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 256
#define MAX_CLIENTS 10

struct msgbuf {
    long mtype; // тип сообщения (тип клиента)
    char mtext[MSG_SIZE];
};

int main() {
    key_t key = ftok("msgqfile", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Создаем или подключаемся к очереди сообщений
    int msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидание сообщений...\n");

    struct msgbuf message;
    int num_clients = 0;
    int client_ids[MAX_CLIENTS];

    // Основной цикл обработки сообщений
    while (1) {
        // Получаем сообщение от любого клиента (любой тип сообщения)
        if (msgrcv(msqid, &message, sizeof(message.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Если сообщение типа 255, то это завершающее сообщение от клиента
        if (message.mtype == 255) {
            printf("Получено завершающее сообщение от клиента.\n");
            // TODO: Удалить клиента из списка, если нужно
            continue;
        }

        // Если клиент новый, добавляем его в список
        int new_client = 1;
        for (int i = 0; i < num_clients; ++i) {
            if (client_ids[i] == message.mtype) {
                new_client = 0;
                break;
            }
        }
        if (new_client) {
            client_ids[num_clients++] = message.mtype;
            printf("Клиент с ID %ld подключен.\n", message.mtype);
        }

        // Отправляем сообщение от клиента всем остальным клиентам
        printf("Получено сообщение от клиента с ID %ld: %s\n", message.mtype, message.mtext);
        for (int i = 0; i < num_clients; ++i) {
            if (client_ids[i] != message.mtype) { // не отправляем сообщение отправителю
                struct msgbuf outgoing_message;
                outgoing_message.mtype = client_ids[i];
                snprintf(outgoing_message.mtext, sizeof(outgoing_message.mtext), "%s", message.mtext);
                if (msgsnd(msqid, &outgoing_message, sizeof(outgoing_message.mtext), 0) == -1) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Удаляем очередь сообщений перед выходом
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
