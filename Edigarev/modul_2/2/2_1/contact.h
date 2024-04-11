//contact.h
#ifndef CONTACT_H
#define CONTACT_H
#pragma once
#include "phonenumber.h"
#include "sociallink.h"
#include "email.h"
#define MAX_CONTACTS 10
#define MAX_NAME_LENGTH 10
#define MAX_PHONE_NUMBERS 3
#define MAX_SOCIAL_LINKS 3
#define MAX_EMAILS 3

typedef struct {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char jobPosition[MAX_NAME_LENGTH];
    char workplace[MAX_NAME_LENGTH];
    PhoneNumber phoneNumbers [MAX_PHONE_NUMBERS];
    int numphoneNumbers;
    SocialLink socialLinks[MAX_SOCIAL_LINKS];
    int numsocialLinks;
    Email emails[MAX_EMAILS];
    int numemails;
} Contact;

void initContact(Contact *contact);         //функция инициализация контакта
void createContact(Contact *contact);       //функция создание контакта
void DeleteContact(Contact *contact);       //функция удаление контакта
void editContact(Contact *contact);         //функция изменение контакта

void editphoneNumbers(Contact *contact);    //функция для изменения телефонных номеров с динамическим меню: добавление, изменение, удаление.
void addphoneNumbers(Contact *contact);     //функция для добавления номера
void editphoneNumber(Contact *contact);     //функция изменения номера
void deletephoneNumbers(Contact *contact);  //функция удаления номера

void editsocialLinks(Contact *contact);     //функция для изменения социальных ссылок: добавление, изменение, удаление.
void addsociallinks(Contact *contact);      //функция для социальной ссылки
void editsociallink(Contact *contact);      //функция изменения социальной ссылоки
void deletesocialLinks(Contact *contact);   //функция удаления социальной ссылоки

void editemails(Contact *contact);    //функция для изменения телефонных номеров: добавление, изменение, удаление.
void addemails(Contact *contact);     //функция для добавления номера
void editemail(Contact *contact);     //функция изменения номера
void deleteemails(Contact *contact);  //функция удаления номера
void cleaninput(char *input);
#endif  /* CONTACT_H */