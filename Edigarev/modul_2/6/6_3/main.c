#include <stdio.h>
#include <dlfcn.h>
#include <stdint.h>

typedef int (*math_func_t)(int, int);

int main() {
    void *handle;
    math_func_t func;

    // Загрузка библиотеки add
    handle = dlopen("./libs/libadd.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    func = (math_func_t)(intptr_t)dlsym(handle, "add_func");
    if (!func) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    int result = func(2, 3);
    printf("Результат (add): %d\n", result);
    dlclose(handle);

    // Загрузка библиотеки sub
    handle = dlopen("./libs/libsub.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    func = (math_func_t)(intptr_t)dlsym(handle, "sub_func");
    if (!func) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    result = func(2, 3);
    printf("Результат (sub): %d\n", result);
    dlclose(handle);

    // Загрузка библиотеки mul
    handle = dlopen("./libs/libmul.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    func = (math_func_t)(intptr_t)dlsym(handle, "mul_func");
    if (!func) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    result = func(2, 3);
    printf("Результат (mul): %d\n", result);
    dlclose(handle);

    // Загрузка библиотеки div
    handle = dlopen("./libs/libdiv.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    func = (math_func_t)(intptr_t)dlsym(handle, "div_func");
    if (!func) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        return 1;
    }
    result = func(2, 3);
    printf("Результат (div): %d\n", result);
    dlclose(handle);

    return 0;
}
