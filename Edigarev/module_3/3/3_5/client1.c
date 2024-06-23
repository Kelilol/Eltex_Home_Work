#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>

#define QUEUE1_NAME "/chat_queue1" // Очередь для отправки сообщений от client1 к client2
#define QUEUE2_NAME "/chat_queue2" // Очередь для получения сообщений от client2 к client1
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

int main() {
    mqd_t mq1, mq2;
    struct mq_attr attr;
    char buffer[MSG_BUFFER_SIZE];
    ssize_t bytes_read;
    int terminate = 0;

    // Устанавливаем атрибуты очереди сообщений
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Открываем очереди сообщений
    mq1 = mq_open(QUEUE1_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq1 == (mqd_t)-1) {
        perror("mq_open mq1");
        exit(1);
    }

    mq2 = mq_open(QUEUE2_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, 0666, &attr);
    if (mq2 == (mqd_t)-1) {
        perror("mq_open mq2");
        exit(1);
    }

    printf("Чат запущен. Введите 'exit' для выхода.\n");

    while (!terminate) {
        // Чтение сообщения из очереди
        memset(buffer, 0, MSG_BUFFER_SIZE);
        bytes_read = mq_receive(mq2, buffer, MSG_BUFFER_SIZE, NULL);
        if (bytes_read >= 0) {
            printf("Client2: %s\n", buffer);
            if (strncmp(buffer, "exit", 4) == 0) {
                terminate = 1;
            }
        } else if (errno != EAGAIN) {
            perror("mq_receive");
        }

        // Ввод и отправка сообщения
        if (!terminate) {
            printf("Client1: ");
            fflush(stdout);
            memset(buffer, 0, MSG_BUFFER_SIZE);
            if (fgets(buffer, MAX_MSG_SIZE, stdin) == NULL) {
                perror("fgets");
                continue;
            }

            buffer[strcspn(buffer, "\n")] = '\0';
            if (strncmp(buffer, "exit", 4) == 0) {
                terminate = 1;
            }

            if (mq_send(mq1, buffer, strlen(buffer) + 1, 0) == -1) {
                perror("mq_send");
            }
        }
    }

    // Закрытие очередей сообщений
    if (mq_close(mq1) == -1) {
        perror("mq_close mq1");
        exit(2);
    }
    if (mq_close(mq2) == -1) {
        perror("mq_close mq2");
        exit(2);
    }

    // Удаление очередей сообщений
    if (mq_unlink(QUEUE1_NAME) == -1) {
        perror("mq_unlink QUEUE1_NAME");
        exit(3);
    }
    if (mq_unlink(QUEUE2_NAME) == -1) {
        perror("mq_unlink QUEUE2_NAME");
        exit(3);
    }

    return 0;
}
