// chat_utils.h
#ifndef CHAT_UTILS_H
#define CHAT_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SERVER_QUEUE_NAME "/chat_server_queue"
#define CLIENT_QUEUE_NAME_TEMPLATE "/chat_client_queue_%d"
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

typedef struct {
    char client_id[20];
    char message[MAX_MSG_SIZE];
} chat_message;

void handle_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

#endif
