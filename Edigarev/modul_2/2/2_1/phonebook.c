//phonebook.c
#include "phonebook.h"

void initPhonebook(Phonebook *phonebook){
phonebook->numContacts=0;

}
void cleaninput(char *input){// Проверяем переполнение буфера
    if (strchr(input, '\n') == NULL) {
        // Очищаем остаток буфера
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        // Удаляем символ новой строки, если он присутствует
        input[strcspn(input, "\n")] = '\0';
    }
}

void addContact(Phonebook *phonebook) {
  
    if (phonebook->numContacts >= MAX_CONTACTS) {
        printf("Phone book is full\n");
        return;
    }

    Contact newContact;
    initContact(&newContact);
    createContact(&newContact);
 
    phonebook->contacts[ phonebook->numContacts++] = newContact;
    printf("Contact added successfully\n");
}

void editContacts(Phonebook *phonebook) {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    printf("Enter first name of contact to edit: ");
    fgets(firstName, 2, stdin);
    cleaninput(firstName);
    //scanf("%s", firstName);
    printf("Enter last name of contact to edit: ");
    fgets(lastName, 2, stdin);
    cleaninput(lastName);

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
    editContact(&phonebook->contacts[foundIndex]);
    printf("Contact updated successfully\n");
}

void deleteContact(Phonebook *phonebook) {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    printf("Enter first name of contact to delete: ");
    fgets(firstName, 2, stdin);
    cleaninput(firstName);
    printf("Enter last name of contact to delete: ");
    fgets(lastName, 2, stdin);
    cleaninput(lastName);

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
    DeleteContact(&phonebook->contacts[foundIndex]);
    for (int i = foundIndex; i < phonebook->numContacts - 1; ++i) {
        phonebook->contacts[i] = phonebook->contacts[i + 1];
    }
    DeleteContact(&phonebook->contacts[phonebook->numContacts]);
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
        for (int j = 0; j < contact.numphoneNumbers; ++j) {
            if (strlen(contact.phoneNumbers[j].phoneNumber) > 0) {
                printf("Phone %d: %s\n", j + 1, contact.phoneNumbers[j].phoneNumber);
            }
        }
        for (int j = 0; j < contact.numsocialLinks; ++j) {
            if (strlen(contact.socialLinks[j].socialLink) > 0) {
                printf("Social Link %d: %s\n", j + 1, contact.socialLinks[j].socialLink);
            }
        }
        for (int j = 0; j < contact.numemails; ++j) {
            if (strlen(contact.emails[j].email) > 0) {
                printf("Email %d: %s\n", j + 1, contact.emails[j].email);
            }
        }
        printf("\n");
    }
}
