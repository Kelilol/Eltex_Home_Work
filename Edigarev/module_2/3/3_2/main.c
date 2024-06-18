#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define MAX_IP_LENGTH 16

int main(int argc, char *argv[]) {
    // Проверяем количество аргументов командной строки
    if (argc != 4) {
        printf("Использование: %s gateway_ip subnet_mask num_packets\n", argv[0]);
        return 1;
    }

    // Получаем IP адрес шлюза, маску подсети и количество пакетов
    char *gateway_ip_str = argv[1];
    char *subnet_mask_str = argv[2];
    int num_packets = atoi(argv[3]);

    // Преобразуем IP адрес шлюза и маску подсети в структуры in_addr
    struct in_addr gateway_ip;
    if (inet_pton(AF_INET, gateway_ip_str, &gateway_ip) != 1) {
        printf("Некорректный IP адрес шлюза\n");
        return 1;
    }

    struct in_addr subnet_mask;
    if (inet_pton(AF_INET, subnet_mask_str, &subnet_mask) != 1) {
        printf("Некорректная маска подсети\n");
        return 1;
    }

    // Вычисляем сетевой адрес подсети
    struct in_addr network_address;
    network_address.s_addr = gateway_ip.s_addr & subnet_mask.s_addr;

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    // Переменные для подсчета пакетов, адресов назначения и сетевых адресов назначения
    int own_subnet_count = 0;
    int other_subnet_count = 0;

    // Генерируем и обрабатываем случайные пакеты
    for (int i = 0; i < num_packets; i++) {
        char random_ip[MAX_IP_LENGTH];
        sprintf(random_ip, "%d.%d.%d.%d", rand() % 256, rand() % 256, rand() % 256, rand() % 256);

        struct in_addr destination_ip;
        if (inet_pton(AF_INET, random_ip, &destination_ip) != 1) {
            printf("Некорректный IP адрес назначения\n");
            return 1;
        }

        struct in_addr destination_network_address;
        destination_network_address.s_addr = destination_ip.s_addr & subnet_mask.s_addr;

        // Проверяем принадлежность адреса назначения к подсети и увеличиваем соответствующий счетчик
        if (destination_network_address.s_addr == network_address.s_addr) {
            own_subnet_count++;
        } else {
            other_subnet_count++;
        }
    }

    // Вычисляем процентное соотношение пакетов и выводим статистику
    int total_packets = own_subnet_count + other_subnet_count;
    float own_subnet_percentage = (float)own_subnet_count / total_packets * 100;
    float other_subnet_percentage = (float)other_subnet_count / total_packets * 100;

    printf("Всего пакетов: %d\n", total_packets);
    printf("Пакетов, предназначенных для своей подсети: %d (%.2f%%)\n", own_subnet_count, own_subnet_percentage);
    printf("Пакетов, предназначенных для других подсетей: %d (%.2f%%)\n", other_subnet_count, other_subnet_percentage);

    return 0;
}