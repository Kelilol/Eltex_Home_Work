#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define PORT 5000
#define MAX_CLIENTS 2
#define MAX_EVENTS 10

void perform_operation(char* input, char* output) {
    int num1, num2;
    char operation[4];
    sscanf(input, "%d %d %s", &num1, &num2, operation);

    if (strcmp(operation, "add") == 0) {
        sprintf(output, "%d", num1 + num2);
    } else if (strcmp(operation, "sub") == 0) {
        sprintf(output, "%d", num1 - num2);
    } else if (strcmp(operation, "mul") == 0) {
        sprintf(output, "%d", num1 * num2);
    } else if (strcmp(operation, "div") == 0) {
        if (num2 != 0) {
            sprintf(output, "%d", num1 / num2);
        } else {
            strcpy(output, "Error: Division by zero");
        }
    } else {
        strcpy(output, "Error: Unknown operation");
    }
}

int main() {
    int server_fd, client_sockets[MAX_CLIENTS], max_clients = MAX_CLIENTS;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char result[1024] = {0};

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind server socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for connections...\n");

    // Create epoll instance
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event, events[MAX_EVENTS];
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    // Add server socket to epoll
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_events; i++) {
            int fd = events[i].data.fd;

            if (fd == server_fd) {
                // New incoming connection
                int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                if (new_socket == -1) {
                    perror("accept");
                    continue;
                }

                printf("New connection. Socket fd is %d\n", new_socket);

                // Add new socket to epoll
                event.events = EPOLLIN;
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }

                // Initialize buffer and result for new client
                memset(buffer, 0, sizeof(buffer));
                memset(result, 0, sizeof(result));
            } else {
                // Data from existing client
                memset(buffer, 0, sizeof(buffer));
                int valread = read(fd, buffer, sizeof(buffer));

                if (valread <= 0) {
                    // Client disconnected
                    getpeername(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("Host disconnected. Ip %s, Port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    // Close the socket and remove from epoll
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                } else {
                    // Process received data
                    printf("Received: %s\n", buffer);
                    perform_operation(buffer, result);
                    send(fd, result, strlen(result), 0);
                    printf("Result sent: %s\n", result);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
