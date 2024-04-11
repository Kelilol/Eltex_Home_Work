// sociallink.c
#include "sociallink.h"

void initSocialLink(SocialLink *socialLink){
    memset(socialLink->socialLink, '\0', sizeof(char)*(MAX_NAME_LENGTH));
}

void addSocialLink(SocialLink *socialLink){
    

    fgets(socialLink->socialLink, MAX_NAME_LENGTH, stdin);
    
    // Проверяем переполнение буфера
    if (strchr(socialLink->socialLink, '\n') == NULL) {
        // Очищаем остаток буфера
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        // Удаляем символ новой строки, если он присутствует
        socialLink->socialLink[strcspn(socialLink->socialLink, "\n")] = '\0';
    }
}


void DeleteSocialLink(SocialLink *socialLink){
    memset(socialLink->socialLink, '\0', sizeof(char)*(MAX_NAME_LENGTH));
}

