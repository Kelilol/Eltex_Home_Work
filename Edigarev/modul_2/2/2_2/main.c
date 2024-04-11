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

int main(void) {
    int choice;
    choice=0;

    Phonebook kek;
    initPhonebook(&kek);

   printMenu();
    return 0;
}
