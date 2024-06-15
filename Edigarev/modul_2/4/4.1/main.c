#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения информации о контакте
typedef struct Contact {
    char lastName[50];
    char firstName[50];
    char middleName[50];
    char workPlace[100];
    char position[100];
    char phoneNumber[20];
    char email[100];
    char socialMedia[100];
    char messengerProfile[100];
} Contact;

// Узел двухсвязного списка
typedef struct Node {
    Contact contact;
    struct Node* prev;
    struct Node* next;
} Node;

// Функция для создания нового контакта
Contact createContact(const char* lastName, const char* firstName, const char* middleName,
                      const char* workPlace, const char* position, const char* phoneNumber,
                      const char* email, const char* socialMedia, const char* messengerProfile) {
    Contact contact;
    strcpy(contact.lastName, lastName);
    strcpy(contact.firstName, firstName);
    strcpy(contact.middleName, middleName);
    strcpy(contact.workPlace, workPlace);
    strcpy(contact.position, position);
    strcpy(contact.phoneNumber, phoneNumber);
    strcpy(contact.email, email);
    strcpy(contact.socialMedia, socialMedia);
    strcpy(contact.messengerProfile, messengerProfile);
    return contact;
}

// Функция для создания нового узла
Node* createNode(Contact contact) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->contact = contact;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Функция для вставки узла в упорядоченный двухсвязный список
void insertNode(Node** head, Contact contact) {
    Node* newNode = createNode(contact);
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node* current = *head;
    while (current != NULL && strcmp(current->contact.lastName, contact.lastName) < 0) {
        current = current->next;
    }

    if (current == NULL) {
        Node* last = *head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newNode;
        newNode->prev = last;
    } else {
        if (current->prev != NULL) {
            current->prev->next = newNode;
        } else {
            *head = newNode;
        }
        newNode->prev = current->prev;
        newNode->next = current;
        current->prev = newNode;
    }
}

// Функция для удаления контакта
void deleteNode(Node** head, const char* lastName, const char* firstName) {
    Node* current = *head;
    while (current != NULL) {
        if (strcmp(current->contact.lastName, lastName) == 0 && strcmp(current->contact.firstName, firstName) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                *head = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            free(current);
            return;
        }
        current = current->next;
    }
}

// Функция для редактирования контакта
void editNode(Node* head, const char* lastName, const char* firstName, Contact newContact) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->contact.lastName, lastName) == 0 && strcmp(current->contact.firstName, firstName) == 0) {
            current->contact = newContact;
            return;
        }
        current = current->next;
    }
}

// Функция для отображения всех контактов
void displayContacts(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("Фамилия: %s\n", current->contact.lastName);
        printf("Имя: %s\n", current->contact.firstName);
        printf("Отчество: %s\n", current->contact.middleName);
        printf("Место работы: %s\n", current->contact.workPlace);
        printf("Должность: %s\n", current->contact.position);
        printf("Номер телефона: %s\n", current->contact.phoneNumber);
        printf("Электронная почта: %s\n", current->contact.email);
        printf("Социальные сети: %s\n", current->contact.socialMedia);
        printf("Профиль в мессенджере: %s\n\n", current->contact.messengerProfile);
        current = current->next;
    }
}

int main() {
    Node* head = NULL;

    Contact contact1 = createContact("Иванов", "Иван", "Иванович", "Компания А", "Менеджер", "89001234567", "ivanov@example.com", "ivanov_vk", "ivanov_telegram");
    Contact contact2 = createContact("Петров", "Петр", "Петрович", "Компания Б", "Директор", "89007654321", "petrov@example.com", "petrov_vk", "petrov_telegram");

    insertNode(&head, contact1);
    insertNode(&head, contact2);

    displayContacts(head);

    Contact newContact = createContact("Иванов", "Иван", "Иванович", "Компания С", "Инженер", "89001112233", "ivanov_new@example.com", "ivanov_new_vk", "ivanov_new_telegram");
    editNode(head, "Иванов", "Иван", newContact);

    printf("После редактирования:\n");
    displayContacts(head);

    deleteNode(&head, "Петров", "Петр");

    printf("После удаления:\n");
    displayContacts(head);

    return 0;
}

