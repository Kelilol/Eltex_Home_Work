#include "contacts_list.h"
#include <stdio.h>
#include <string.h>

int main() {
    ContactList* list = createContactList();

    Contact contact1 = {"Иванов", "Иван", "Иванович", "Компания А", "Менеджер", "89001234567", "ivanov@example.com", "ivanov_vk", "ivanov_telegram"};
    Contact contact2 = {"Петров", "Петр", "Петрович", "Компания Б", "Директор", "89007654321", "petrov@example.com", "petrov_vk", "petrov_telegram"};
    Contact contact3 = {"Сидоров", "Сидор", "Сидорович", "Компания В", "Инженер", "89001234568", "sidorov@example.com", "sidorov_vk", "sidorov_telegram"};

    addContact(list, contact1);
    addContact(list, contact2);
    addContact(list, contact3);

    printf("Контакты после добавления:\n");
    displayContacts(list);

    deleteContact(list, "Петров");

    printf("Контакты после удаления Петрова:\n");
    displayContacts(list);

    Contact newContact = {"Сидоров", "Сидор", "Сидорович", "Компания Г", "Техник", "89001234569", "new_sidorov@example.com", "new_sidorov_vk", "new_sidorov_telegram"};
    editContact(list, "Сидоров", newContact);

    printf("Контакты после редактирования Сидорова:\n");
    displayContacts(list);

    freeContactList(list);

    return 0;
}
