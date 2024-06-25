#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client1_addr, client2_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Создание UDP-сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Ошибка при создании сокета");
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета к порту
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Ошибка при привязке сокета");
    }

    printf("Сервер запущен. Ожидание клиентов...\n");

    // Инициализация адресов клиентов
    memset(&client1_addr, 0, sizeof(client1_addr));
    memset(&client2_addr, 0, sizeof(client2_addr));

    // Получение сообщений от клиентов и пересылка их другим клиентам
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client1_addr, &addr_len);
        if (n < 0) {
            error("Ошибка при получении сообщения");
        }
        buffer[n] = '\0';
        printf("Сообщение от клиента 1: %s\n", buffer);

        // Пересылка сообщения клиенту 2
        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client2_addr, addr_len);

        // Получение сообщения от клиента 2
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client2_addr, &addr_len);
        if (n < 0) {
            error("Ошибка при получении сообщения");
        }
        buffer[n] = '\0';
        printf("Сообщение от клиента 2: %s\n", buffer);

        // Пересылка сообщения клиенту 1
        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client1_addr, addr_len);
    }

    close(sockfd);
    return 0;
}
