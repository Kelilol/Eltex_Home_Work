// file_io.c
#include "file_io.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void load_contacts_from_file(ContactList *list, const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    Contact contact;
    while (read(fd, &contact, sizeof(Contact)) == sizeof(Contact)) {
        add_contact(list, contact.first_name, contact.last_name, contact.phone, contact.email);
    }
    close(fd);
}

void save_contacts_to_file(ContactList *list, const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }
    Contact *current = list->head;
    while (current) {
        write(fd, current, sizeof(Contact));
        current = current->next;
    }
    close(fd);
}

