//phonebook.h
#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#pragma once
#include "contact.h"
#define MAX_CONTACTS 10
#define MAX_NAME_LENGTH 10
#define MAX_PHONE_NUMBERS 3
#define MAX_SOCIAL_LINKS 3
#define MAX_EMAILS 3

typedef struct {
Contact contacts[MAX_CONTACTS];
int numContacts;
} Phonebook;

void initPhonebook(Phonebook *phonebook);   //функция инициализации телефонной книги
void addContact(Phonebook *phonebook);      //функция добавления контакта
void editContacts(Phonebook *phonebook);     //функция изменения контакта
void deleteContact(Phonebook *phonebook);   //функция удаления контакта
void viewAllContacts(Phonebook *phonebook); //функция для вывода всех контактов
#endif /* PHONEBOOK_H */