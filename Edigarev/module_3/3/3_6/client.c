// client.c
#include "chat_utils.h"

int main() {
    mqd_t server_mq, client_mq;
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    chat_message msg;
    int client_id = getpid(); // Используем PID в качестве идентификатора клиента
    char client_queue_name[64];

    // Устанавливаем атрибуты очереди сообщений
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(chat_message);
    attr.mq_curmsgs = 0;

    // Формируем имя очереди сообщений клиента
    sprintf(client_queue_name, CLIENT_QUEUE_NAME_TEMPLATE, client_id);

    // Открываем очередь сообщений клиента
    client_mq = mq_open(client_queue_name, O_CREAT | O_RDONLY | O_NONBLOCK, 0666, &attr);
    if (client_mq == (mqd_t)-1) {
        handle_error("mq_open client");
    }

    // Открываем очередь сообщений сервера
    server_mq = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (server_mq == (mqd_t)-1) {
        handle_error("mq_open server");
    }

    // Отправляем сообщение о подключении
    snprintf(msg.client_id, 20, "%d", client_id);
    snprintf(msg.message, MAX_MSG_SIZE, "connect");
    if (mq_send(server_mq, (char*)&msg, sizeof(chat_message), 0) == -1) {
        handle_error("mq_send connect");
    }

    printf("Чат запущен. Введите 'exit' для выхода.\n");

    while (1) {
        // Чтение сообщения от сервера
        ssize_t bytes_read = mq_receive(client_mq, (char*)&msg, sizeof(chat_message), NULL);
        if (bytes_read >= 0) {
            printf("%s: %s\n", msg.client_id, msg.message);
        } else {
            if (errno != EAGAIN) {
                handle_error("mq_receive");
            }
        }

        // Ввод и отправка сообщения
        printf("Вы: ");
        fflush(stdout);
        if (fgets(buffer, MAX_MSG_SIZE, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (strcmp(buffer, "exit") == 0) {
                snprintf(msg.message, MAX_MSG_SIZE, "disconnect");
                if (mq_send(server_mq, (char*)&msg, sizeof(chat_message), 0) == -1) {
                    handle_error("mq_send disconnect");
                }
                break;
            }
            snprintf(msg.message, MAX_MSG_SIZE, "%s", buffer);
            if (mq_send(server_mq, (char*)&msg, sizeof(chat_message), 0) == -1) {
                handle_error("mq_send message");
            }
        }
    }

    // Закрытие очереди сообщений клиента
    if (mq_close(client_mq) == -1) {
        handle_error("mq_close client");
    }

    // Удаление очереди сообщений клиента
    if (mq_unlink(client_queue_name) == -1) {
        handle_error("mq_unlink client");
    }

    return 0;
}
