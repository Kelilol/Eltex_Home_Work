#ifndef CONTACTS_LIST_H
#define CONTACTS_LIST_H

#include <stdbool.h>

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

// Узел двусвязного списка
typedef struct ListNode {
    Contact contact;
    struct ListNode* prev;
    struct ListNode* next;
} ListNode;

// Двусвязный список
typedef struct {
    ListNode* head;
    ListNode* tail;
} ContactList;

// Функции для работы с двусвязным списком
ContactList* createContactList();
void addContact(ContactList* list, Contact contact);
bool deleteContact(ContactList* list, const char* lastName);
void editContact(ContactList* list, const char* lastName, Contact newContact);
void displayContacts(const ContactList* list);
void freeContactList(ContactList* list);

#endif // CONTACTS_LIST_H
