#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000

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
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char result[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for connections...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, sizeof(buffer));
        if (valread <= 0) {
            break;
        }

        printf("Received: %s\n", buffer);
        
        perform_operation(buffer, result);
        send(new_socket, result, strlen(result), 0);
        printf("Result sent: %s\n", result);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
