#include "operationCulc.h"





int main() {
    char operation[2];
    char num_str[10];
    int index;
    float num1, num2, result;

    // Массив указателей на функции
    operation_ptr operations;
    initoperation_ptr(operations);
    // Цикл для непрерывного выполнения операций
    while (1) {
        printf("Введите операцию (+, -, *, /) или q для выхода: ");
        fgets(operation, 2, stdin);
        cleaninput(operation);
        // Проверка на выход из программы
        if (operation[0] == 'q') {
            printf("До свидания!\n");
            break;
        }

        // Ввод чисел
        printf("Введите первое число числа: ");
        fgets(num_str, 10, stdin);
        cleaninput(num_str);
        num1 = atof(num_str);
        printf("Введите второе число числа: ");
        fgets(num_str, 10, stdin);
        cleaninput(num_str);
        num2 = atof(num_str);
        
        

        // Поиск соответствующей функции и вызов
        switch (operation[0]) {
            case '+':
            index=0;
            break;
            case '-':
            index=1;
            break;
            case '*':
            index=2;
            break;
            case '/':
            index=3;
            break;
            default:
                printf("Ошибка: неверная операция\n");
        }
        result = operations[index](num1, num2);
        printf("Результат: %.2f\n", result);
    }

    return 0;
}


