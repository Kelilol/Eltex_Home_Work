// Определения функций для калькулятора
#include "operationCulc.h"

float add(float a, float b) {
    return a + b;
}

float subtract(float a, float b) {
    return a - b;
}

float multiply(float a, float b) {
    return a * b;
}

float divide(float a, float b) {
    if (b == 0) {
        printf("Ошибка: деление на ноль\n");
        return 0;
    }
    return a / b;
}

void initoperation_ptr(operation_ptr operations)
{
    operations[0]=add;
    operations[1]=subtract;
    operations[2]=multiply;
    operations[3]=divide;
}

void cleaninput(char *input){// Проверяем переполнение буфера
    if (strchr(input, '\n') == NULL) {
        // Очищаем остаток буфера
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    } else {
        // Удаляем символ новой строки, если он присутствует
        input[strcspn(input, "\n")] = '\0';
    }
}