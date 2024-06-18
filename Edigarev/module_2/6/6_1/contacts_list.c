#include "contacts_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Создание нового списка контактов
ContactList* createContactList() {
    ContactList* list = (ContactList*)malloc(sizeof(ContactList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

// Добавление контакта в список
void addContact(ContactList* list, Contact contact) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->contact = contact;
    newNode->prev = NULL;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        ListNode* current = list->head;
        while (current != NULL && strcmp(current->contact.lastName, contact.lastName) < 0) {
            current = current->next;
        }

        if (current == NULL) {
            list->tail->next = newNode;
            newNode->prev = list->tail;
            list->tail = newNode;
        } else {
            newNode->next = current;
            newNode->prev = current->prev;
            if (current->prev != NULL) {
                current->prev->next = newNode;
            } else {
                list->head = newNode;
            }
            current->prev = newNode;
        }
    }
}

// Удаление контакта из списка по фамилии
bool deleteContact(ContactList* list, const char* lastName) {
    ListNode* current = list->head;
    while (current != NULL && strcmp(current->contact.lastName, lastName) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        return false;
    }

    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        list->tail = current->prev;
    }

    free(current);
    return true;
}

// Редактирование контакта по фамилии
void editContact(ContactList* list, const char* lastName, Contact newContact) {
    ListNode* current = list->head;
    while (current != NULL && strcmp(current->contact.lastName, lastName) != 0) {
        current = current->next;
    }

    if (current != NULL) {
        current->contact = newContact;
    }
}

// Отображение всех контактов в списке
void displayContacts(const ContactList* list) {
    ListNode* current = list->head;
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

// Освобождение памяти списка контактов
void freeContactList(ContactList* list) {
    ListNode* current = list->head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}
