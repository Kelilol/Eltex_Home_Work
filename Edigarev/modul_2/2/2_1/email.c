//email.c
#include "email.h"


void initEmail(Email*email){
memset(email->email,   '\0', sizeof(char)*(MAX_NAME_LENGTH));
}

void addEmail(Email*email){

    fgets(email->email, MAX_NAME_LENGTH, stdin);
    
    // Проверяем переполнение буфера
    if (strchr(email->email, '\n') == NULL) {
        // Очищаем остаток буфера
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        // Удаляем символ новой строки, если он присутствует
        email->email[strcspn(email->email, "\n")] = '\0';
    }
}

void DeleteEmail(Email*email){
memset(email->email,   '\0', sizeof(char)*(MAX_NAME_LENGTH));
}
