#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    struct sockaddr_in client1_addr, client2_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int client1_connected = 0, client2_connected = 0;

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Привязка сокета к адресу сервера
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", port);

    while (1) {
        // Получение данных от клиентов
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0) {
            perror("recvfrom");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[n] = '\0';
        printf("Received message from client: %s\n", buffer);

        // Определение клиента и сохранение его адреса
        if (!client1_connected) {
            client1_addr = client_addr;
            client1_connected = 1;
            printf("Client 1 connected\n");
        } else if (client1_connected && !client2_connected &&
                   (client_addr.sin_addr.s_addr != client1_addr.sin_addr.s_addr || 
                    client_addr.sin_port != client1_addr.sin_port)) {
            client2_addr = client_addr;
            client2_connected = 1;
            printf("Client 2 connected\n");
        }

        // Пересылка сообщения между клиентами
        if (client1_connected && client2_connected) {
            if (client_addr.sin_addr.s_addr == client1_addr.sin_addr.s_addr && 
                client_addr.sin_port == client1_addr.sin_port) {
                sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client2_addr, addr_len);
                printf("Forwarded message to Client 2\n");
            } else {
                sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client1_addr, addr_len);
                printf("Forwarded message to Client 1\n");
            }
        }
    }

    close(sockfd);
    return 0;
}

