// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact_list.h"
#include "file_io.h"

#define FILENAME "contacts.dat"

void print_contacts(ContactList *list) {
    Contact *current = list->head;
    while (current) {
        printf("Name: %s %s, Phone: %s, Email: %s\n",
               current->first_name, current->last_name,
               current->phone, current->email);
        current = current->next;
    }
}

int main() {
    ContactList list;
    init_contact_list(&list);
    load_contacts_from_file(&list, FILENAME);

    char command[50];
    char first_name[50], last_name[50], phone[20], email[50];

    while (1) {
        printf("Enter command (add/edit/delete/print/exit): ");
        scanf("%s", command);

        if (strcmp(command, "add") == 0) {
            printf("First Name: ");
            scanf("%s", first_name);
            printf("Last Name: ");
            scanf("%s", last_name);
            printf("Phone: ");
            scanf("%s", phone);
            printf("Email: ");
            scanf("%s", email);
            add_contact(&list, first_name, last_name, phone, email);
        } else if (strcmp(command, "edit") == 0) {
            printf("First Name: ");
            scanf("%s", first_name);
            printf("Last Name: ");
            scanf("%s", last_name);
            Contact *contact = find_contact(&list, first_name, last_name);
            if (contact) {
                printf("New Phone: ");
                scanf("%s", phone);
                printf("New Email: ");
                scanf("%s", email);
                edit_contact(contact, NULL, NULL, phone, email);
            } else {
                printf("Contact not found.\n");
            }
        } else if (strcmp(command, "delete") == 0) {
            printf("First Name: ");
            scanf("%s", first_name);
            printf("Last Name: ");
            scanf("%s", last_name);
            Contact *contact = find_contact(&list, first_name, last_name);
            if (contact) {
                delete_contact(&list, contact);
            } else {
                printf("Contact not found.\n");
            }
        } else if (strcmp(command, "print") == 0) {
            print_contacts(&list);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    save_contacts_to_file(&list, FILENAME);
    free_contact_list(&list);
    return 0;
}

