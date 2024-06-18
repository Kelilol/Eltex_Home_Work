#include <stdio.h>
#include <stdlib.h>

#define MAX_PRIORITY 256

typedef struct Node {
    int value;
    int priority;
    struct Node* next;
} Node;

typedef struct PriorityQueue {
    Node* head;
} PriorityQueue;

PriorityQueue* createQueue() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->head = NULL;
    return pq;
}

void enqueue(PriorityQueue* pq, int value, int priority) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->priority = priority;
    newNode->next = NULL;

    if (pq->head == NULL || pq->head->priority > priority) {
        newNode->next = pq->head;
        pq->head = newNode;
    } else {
        Node* current = pq->head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

int dequeue(PriorityQueue* pq) {
    if (pq->head == NULL) {
        printf("Queue is empty.\n");
        return -1;
    }
    Node* temp = pq->head;
    int value = temp->value;
    pq->head = pq->head->next;
    free(temp);
    return value;
}

int dequeueWithPriority(PriorityQueue* pq, int priority) {
    if (pq->head == NULL) {
        printf("Queue is empty.\n");
        return -1;
    }
    Node* current = pq->head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->priority == priority) {
            if (prev == NULL) {
                pq->head = current->next;
            } else {
                prev->next = current->next;
            }
            int value = current->value;
            free(current);
            return value;
        }
        prev = current;
        current = current->next;
    }
    printf("No element with the specified priority.\n");
    return -1;
}

int dequeueWithMinPriority(PriorityQueue* pq, int minPriority) {
    if (pq->head == NULL) {
        printf("Queue is empty.\n");
        return -1;
    }
    Node* current = pq->head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->priority >= minPriority) {
            if (prev == NULL) {
                pq->head = current->next;
            } else {
                prev->next = current->next;
            }
            int value = current->value;
            free(current);
            return value;
        }
        prev = current;
        current = current->next;
    }
    printf("No element with the specified minimum priority.\n");
    return -1;
}

void displayQueue(PriorityQueue* pq) {
    Node* current = pq->head;
    while (current != NULL) {
        printf("Value: %d, Priority: %d\n", current->value, current->priority);
        current = current->next;
    }
}

int main() {
    PriorityQueue* pq = createQueue();

    // Имитируем генерацию сообщений с различными приоритетами
    enqueue(pq, 10, 1);
    enqueue(pq, 20, 3);
    enqueue(pq, 30, 2);
    enqueue(pq, 40, 0);
    enqueue(pq, 50, 2);

    printf("Queue after enqueue operations:\n");
    displayQueue(pq);

    // Извлечение элемента, находящегося первым в очереди
    int value = dequeue(pq);
    printf("\nDequeued element with highest priority: %d\n", value);

    printf("Queue after dequeue operation:\n");
    displayQueue(pq);

    // Извлечение элемента с указанным приоритетом
    value = dequeueWithPriority(pq, 2);
    printf("\nDequeued element with priority 2: %d\n", value);

    printf("Queue after dequeueWithPriority operation:\n");
    displayQueue(pq);

    // Извлечение элемента с приоритетом не ниже чем заданный
    value = dequeueWithMinPriority(pq, 1);
    printf("\nDequeued element with priority not lower than 1: %d\n", value);

    printf("Queue after dequeueWithMinPriority operation:\n");
    displayQueue(pq);

    // Очистка памяти
    while (pq->head != NULL) {
        dequeue(pq);
    }
    free(pq);

    return 0;
}

