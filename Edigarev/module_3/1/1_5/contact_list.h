// contact_list.h
#ifndef CONTACT_LIST_H
#define CONTACT_LIST_H

#include <stdlib.h>
#include <string.h>

typedef struct Contact {
    char first_name[50];
    char last_name[50];
    char phone[20];
    char email[50];
    struct Contact *prev;
    struct Contact *next;
} Contact;

typedef struct {
    Contact *head;
    Contact *tail;
} ContactList;

void init_contact_list(ContactList *list);
void add_contact(ContactList *list, const char *first_name, const char *last_name, const char *phone, const char *email);
void edit_contact(Contact *contact, const char *first_name, const char *last_name, const char *phone, const char *email);
void delete_contact(ContactList *list, Contact *contact);
void free_contact_list(ContactList *list);
Contact* find_contact(ContactList *list, const char *first_name, const char *last_name);

#endif // CONTACT_LIST_H

