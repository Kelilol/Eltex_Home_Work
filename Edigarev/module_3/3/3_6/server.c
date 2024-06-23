// server.c
#include "chat_utils.h"

int main() {
    mqd_t server_mq;
    struct mq_attr attr;
    chat_message msg;
    int client_count = 0;
    mqd_t client_mqs[MAX_MESSAGES];

    // Устанавливаем атрибуты очереди сообщений
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(chat_message);
    attr.mq_curmsgs = 0;

    // Открываем очередь сообщений сервера
    server_mq = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (server_mq == (mqd_t)-1) {
        handle_error("mq_open server");
    }

    printf("Сервер запущен. Ожидание сообщений...\n");

    while (1) {
        // Чтение сообщения от клиента
        ssize_t bytes_read = mq_receive(server_mq, (char*)&msg, sizeof(chat_message), NULL);
        if (bytes_read >= 0) {
            printf("Получено сообщение от клиента %s: %s\n", msg.client_id, msg.message);

            // Пересылка сообщения всем подключенным клиентам
            for (int i = 0; i < client_count; ++i) {
                if (mq_send(client_mqs[i], (char*)&msg, sizeof(chat_message), 0) == -1) {
                    handle_error("mq_send to client");
                }
            }

            // Добавление нового клиента
            if (strncmp(msg.message, "connect", 7) == 0) {
                char client_queue_name[64];
                sprintf(client_queue_name, CLIENT_QUEUE_NAME_TEMPLATE, atoi(msg.client_id));
                client_mqs[client_count] = mq_open(client_queue_name, O_WRONLY);
                if (client_mqs[client_count] == (mqd_t)-1) {
                    handle_error("mq_open client");
                }
                client_count++;
            }

            // Удаление клиента
            if (strncmp(msg.message, "disconnect", 10) == 0) {
                // Реализовать логику отключения клиента
            }
        } else {
            if (errno != EAGAIN) {
                handle_error("mq_receive");
            }
        }
    }

    // Закрытие очереди сообщений сервера
    if (mq_close(server_mq) == -1) {
        handle_error("mq_close server");
    }

    // Удаление очереди сообщений сервера
    if (mq_unlink(SERVER_QUEUE_NAME) == -1) {
        handle_error("mq_unlink server");
    }

    return 0;
}
