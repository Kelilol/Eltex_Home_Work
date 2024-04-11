//phonenumber.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 10
typedef struct {
    char phoneNumber[MAX_NAME_LENGTH];
} PhoneNumber;

void initPhoneNumber(PhoneNumber *phoneNumber);
void addPhoneNumber (PhoneNumber *phoneNumber);
void DeletePhoneNumber (PhoneNumber *phoneNumber);
