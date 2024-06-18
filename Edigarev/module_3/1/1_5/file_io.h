// file_io.h
#ifndef FILE_IO_H
#define FILE_IO_H

#include "contact_list.h"

void load_contacts_from_file(ContactList *list, const char *filename);
void save_contacts_to_file(ContactList *list, const char *filename);

#endif // FILE_IO_H

