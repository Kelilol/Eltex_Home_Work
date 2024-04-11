//email.h
#ifndef EMAIL_H
#define EMAIL_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 10
typedef struct {
    char email[MAX_NAME_LENGTH];
} Email;

void initEmail(Email*email);
void addEmail (Email*email);
void DeleteEmail(Email*email);
#endif /* EMAIL_H */
