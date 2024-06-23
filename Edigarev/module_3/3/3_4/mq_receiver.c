#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define QUEUE_NAME "/my_message_queue" // Имя очереди сообщений
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    ssize_t bytes_read;

    // Устанавливаем атрибуты очереди сообщений
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Открываем очередь сообщений с указанным именем
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Очередь сообщений открыта. Ожидание сообщений из очереди.\n");

    // Чтение сообщений из очереди
    while (1) {
        memset(buffer, 0, MSG_BUFFER_SIZE);
        bytes_read = mq_receive(mq, buffer, MSG_BUFFER_SIZE, NULL);
        if (bytes_read == -1) {
            perror("mq_receive");
            continue;
        }

        printf("Получено сообщение: %s\n", buffer);

        // Проверка на завершение
        if (strncmp(buffer, "255", 3) == 0) {
            printf("Получение сообщений завершено.\n");
            break;
        }
    }

    // Закрываем очередь сообщений
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(2);
    }

    return 0;
}
