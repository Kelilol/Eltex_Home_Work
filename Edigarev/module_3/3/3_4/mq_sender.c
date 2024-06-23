#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define QUEUE_NAME "/my_message_queue" // Имя очереди сообщений
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    int terminate = 0;

    // Устанавливаем атрибуты очереди сообщений
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Открываем очередь сообщений с указанным именем
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Очередь сообщений открыта. Отправка сообщений в очередь.\n");
    printf("Для завершения отправки введите сообщение с типом 255.\n");

    // Отправка сообщений в очередь
    while (!terminate) {
        printf("Введите сообщение: ");
        fflush(stdout);
        memset(buffer, 0, MSG_BUFFER_SIZE);
        if (fgets(buffer, MAX_MSG_SIZE, stdin) == NULL) {
            perror("fgets");
            continue;
        }
        
        // Убираем символ новой строки
        buffer[strcspn(buffer, "\n")] = '\0';

        // Проверка на завершение
        if (strncmp(buffer, "255", 3) == 0) {
            terminate = 1;
            printf("Отправка завершена.\n");
        }

        // Отправка сообщения в очередь
        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send");
            continue;
        }
    }

    // Закрываем очередь сообщений
    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(2);
    }

    // Удаляем очередь сообщений
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(3);
    }

    return 0;
}
