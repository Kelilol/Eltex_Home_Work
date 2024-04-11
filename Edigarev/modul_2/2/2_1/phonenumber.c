//phonenumber.c
#include "phonenumber.h"
#include "string.h"

void initPhoneNumber(PhoneNumber *phoneNumber){
    memset(phoneNumber->phoneNumber,   '\0', sizeof(char)*(MAX_NAME_LENGTH));
}

void addPhoneNumber (PhoneNumber *phoneNumber){

     
     fgets(phoneNumber->phoneNumber, MAX_NAME_LENGTH, stdin);
    
    // Проверяем переполнение буфера
    if (strchr(phoneNumber->phoneNumber, '\n') == NULL) {
        // Очищаем остаток буфера
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        // Удаляем символ новой строки, если он присутствует
        phoneNumber->phoneNumber[strcspn(phoneNumber->phoneNumber, "\n")] = '\0';
    }
}

void DeletePhoneNumber (PhoneNumber *phoneNumber){
    memset(phoneNumber->phoneNumber,   '\0', sizeof(char)*(MAX_NAME_LENGTH));
}
