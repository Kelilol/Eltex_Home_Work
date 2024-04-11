#ifndef OPERATIONCULC_H
#define OPERATIONCULC_H
#define NUM_OPERATIONS 4
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
// Объявления функций для калькулятора
float add(float a, float b);
float subtract(float a, float b);
float multiply(float a, float b);
float divide(float a, float b);

typedef float (*operation_ptr[NUM_OPERATIONS])(float, float);

void initoperation_ptr(operation_ptr operations);
void cleaninput(char *input);
#endif /* OPERATIONCULC_H */