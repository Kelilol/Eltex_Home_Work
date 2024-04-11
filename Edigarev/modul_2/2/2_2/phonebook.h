#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CONTACTS 10
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_NUMBERS 3
#define MAX_SOCIAL_LINKS 3
#define MAX_EMAILS 3

typedef struct {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char jobPosition[MAX_NAME_LENGTH];
    char workplace[MAX_NAME_LENGTH];
    char phoneNumbers[MAX_PHONE_NUMBERS][MAX_NAME_LENGTH];
    char socialLinks[MAX_SOCIAL_LINKS][MAX_NAME_LENGTH];
    char emails[MAX_EMAILS][MAX_NAME_LENGTH];
} Contact;

typedef struct {
Contact contacts[MAX_CONTACTS];
int numContacts;
} Phonebook;

void initPhonebook(Phonebook *phonebook);
void addContact(Phonebook *phonebook);
void editContact(Phonebook *phonebook);
void deleteContact(Phonebook *phonebook);
void viewAllContacts(Phonebook *phonebook);