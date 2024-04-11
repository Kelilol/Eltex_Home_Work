#include "phonebook.h"

void initPhonebook(Phonebook *phonebook){
phonebook->numContacts=0;

}


void addContact(Phonebook *phonebook) {
    if (phonebook->numContacts >= MAX_CONTACTS) {
        printf("Phone book is full\n");
        return;
    }

    Contact newContact;
    printf("Enter first name: ");
    scanf("%s", newContact.firstName);
    printf("Enter last name: ");
    scanf("%s", newContact.lastName);
    printf("Enter job position: ");
    scanf("%s", newContact.jobPosition);
    printf("Enter workplace: ");
    scanf("%s", newContact.workplace);
    for (int i = 0; i < MAX_PHONE_NUMBERS; ++i) {
        printf("Enter phone number %d: ", i + 1);
        scanf("%s", newContact.phoneNumbers[i]);
    }
    for (int i = 0; i < MAX_SOCIAL_LINKS; ++i) {
        printf("Enter social link %d: ", i + 1);
        scanf("%s", newContact.socialLinks[i]);
    }
    for (int i = 0; i < MAX_EMAILS; ++i) {
        printf("Enter email %d: ", i + 1);
        scanf("%s", newContact.emails[i]);
    }
    
    phonebook->contacts[ phonebook->numContacts++] = newContact;
    printf("Contact added successfully\n");
}

void editContact(Phonebook *phonebook) {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    printf("Enter first name of contact to edit: ");
    scanf("%s", firstName);
    printf("Enter last name of contact to edit: ");
    scanf("%s", lastName);

    int foundIndex = -1;
    for (int i = 0; i < phonebook->numContacts; ++i) {
        if (strcmp(firstName, phonebook->contacts[i].firstName) == 0 && strcmp(lastName, phonebook->contacts[i].lastName) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Contact not found\n");
        return;
    }

    Contact *contact = &phonebook->contacts[foundIndex];
    printf("Enter new first name (leave blank to keep unchanged): ");
    scanf("%s", contact->firstName);
    printf("Enter new last name (leave blank to keep unchanged): ");
    scanf("%s", contact->lastName);
    printf("Enter new job position (leave blank to keep unchanged): ");
    scanf("%s", contact->jobPosition);
    printf("Enter new workplace (leave blank to keep unchanged): ");
    scanf("%s", contact->workplace);
    for (int i = 0; i < MAX_PHONE_NUMBERS; ++i) {
        printf("Enter new phone number %d (leave blank to keep unchanged): ", i + 1);
        scanf("%s", contact->phoneNumbers[i]);
    }
    for (int i = 0; i < MAX_SOCIAL_LINKS; ++i) {
        printf("Enter new social link %d (leave blank to keep unchanged): ", i + 1);
        scanf("%s", contact->socialLinks[i]);
    }
    for (int i = 0; i < MAX_EMAILS; ++i) {
        printf("Enter new email %d (leave blank to keep unchanged): ", i + 1);
        scanf("%s", contact->emails[i]);
    }

    printf("Contact updated successfully\n");
}

void deleteContact(Phonebook *phonebook) {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    printf("Enter first name of contact to delete: ");
    scanf("%s", firstName);
    printf("Enter last name of contact to delete: ");
    scanf("%s", lastName);

    int foundIndex = -1;
    for (int i = 0; i < phonebook->numContacts; ++i) {
        if (strcmp(firstName, phonebook->contacts[i].firstName) == 0 && strcmp(lastName, phonebook->contacts[i].lastName) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Contact not found\n");
        return;
    }

    for (int i = foundIndex; i < phonebook->numContacts - 1; ++i) {
        phonebook->contacts[i] = phonebook->contacts[i + 1];
    }
    phonebook->numContacts--;
    printf("Contact deleted successfully\n");
}

void viewAllContacts(Phonebook *phonebook) {
    printf("\nContacts:\n");
    for (int i = 0; i < phonebook->numContacts; ++i) {
        Contact contact = phonebook->contacts[i];
        printf("Name: %s %s\n", contact.firstName, contact.lastName);
        printf("Job Position: %s\n", contact.jobPosition);
        printf("Workplace: %s\n", contact.workplace);
        for (int j = 0; j < MAX_PHONE_NUMBERS; ++j) {
            if (strlen(contact.phoneNumbers[j]) > 0) {
                printf("Phone %d: %s\n", j + 1, contact.phoneNumbers[j]);
            }
        }
        for (int j = 0; j < MAX_SOCIAL_LINKS; ++j) {
            if (strlen(contact.socialLinks[j]) > 0) {
                printf("Social Link %d: %s\n", j + 1, contact.socialLinks[j]);
            }
        }
        for (int j = 0; j < MAX_EMAILS; ++j) {
            if (strlen(contact.emails[j]) > 0) {
                printf("Email %d: %s\n", j + 1, contact.emails[j]);
            }
        }
        printf("\n");
    }
}