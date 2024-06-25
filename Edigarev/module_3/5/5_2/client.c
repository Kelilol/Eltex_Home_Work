#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5000
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Создание UDP-сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Ошибка при создании сокета");
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    printf("Клиент запущен. Введите 'exit' для выхода.\n");

    while (1) {
        printf("Введите сообщение: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Удаление символа новой строки

        // Проверка на завершение работы клиента
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Отправка сообщения серверу
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len) < 0) {
            error("Ошибка при отправке сообщения");
        }

        // Получение сообщения от сервера
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (n < 0) {
            error("Ошибка при получении сообщения");
        }
        buffer[n] = '\0';
        printf("Получено сообщение: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
