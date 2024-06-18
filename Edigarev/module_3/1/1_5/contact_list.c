// contact_list.c
#include "contact_list.h"
#include <stdio.h>

void init_contact_list(ContactList *list) {
    list->head = NULL;
    list->tail = NULL;
}

void add_contact(ContactList *list, const char *first_name, const char *last_name, const char *phone, const char *email) {
    Contact *new_contact = (Contact *)malloc(sizeof(Contact));
    strcpy(new_contact->first_name, first_name);
    strcpy(new_contact->last_name, last_name);
    strcpy(new_contact->phone, phone);
    strcpy(new_contact->email, email);
    new_contact->prev = list->tail;
    new_contact->next = NULL;
    if (list->tail) {
        list->tail->next = new_contact;
    } else {
        list->head = new_contact;
    }
    list->tail = new_contact;
}

void edit_contact(Contact *contact, const char *first_name, const char *last_name, const char *phone, const char *email) {
    if (first_name) strcpy(contact->first_name, first_name);
    if (last_name) strcpy(contact->last_name, last_name);
    if (phone) strcpy(contact->phone, phone);
    if (email) strcpy(contact->email, email);
}

void delete_contact(ContactList *list, Contact *contact) {
    if (contact->prev) {
        contact->prev->next = contact->next;
    } else {
        list->head = contact->next;
    }
    if (contact->next) {
        contact->next->prev = contact->prev;
    } else {
        list->tail = contact->prev;
    }
    free(contact);
}

void free_contact_list(ContactList *list) {
    Contact *current = list->head;
    while (current) {
        Contact *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

Contact* find_contact(ContactList *list, const char *first_name, const char *last_name) {
    Contact *current = list->head;
    while (current) {
        if (strcmp(current->first_name, first_name) == 0 && strcmp(current->last_name, last_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

