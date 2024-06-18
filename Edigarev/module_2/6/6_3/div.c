// div.c
#include "math_func.h"
#include <stdio.h>

int div_func(int a, int b) {
    if (b == 0) {
        printf("Ошибка: деление на ноль!\n");
        return 0;
    }
    return a / b;
}
