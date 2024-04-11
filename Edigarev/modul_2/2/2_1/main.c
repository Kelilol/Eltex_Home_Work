//main.c
#include "phonebook.h"



void printMenu() {
    printf("\nMenu:\n");
    printf("1. Add contact\n");
    printf("2. Edit contact\n");
    printf("3. Delete contact\n");
    printf("4. View all contacts\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    char input[2];
    Phonebook phonebook;
    initPhonebook( &phonebook);
    do {
        printMenu();
        //scanf("%d", &choice);
        fgets(input, 2, stdin);
        cleaninput(input);
        choice=atoi(input);
        switch (choice) {
            case 1:
                addContact(&phonebook);
                break;
            case 2:
                editContacts(&phonebook);
                break;
            case 3:
                deleteContact(&phonebook);
                break;
            case 4:
                viewAllContacts(&phonebook);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);

    return 0;
}
