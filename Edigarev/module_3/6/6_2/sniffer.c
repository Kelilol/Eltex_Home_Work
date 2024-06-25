

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #define __USE_MISC
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>


#define SNAP_LEN 1518
#define SIZE_ETHERNET 14

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ip *iph;
    struct udphdr *udph;
    const u_char *payload;
    int size_ip;
    int size_udp;
    int size_payload;

    // Получение IP заголовка
    iph = (struct ip *)(packet + SIZE_ETHERNET);
    size_ip = iph->ip_hl * 4;
    if (size_ip < 20) {
        fprintf(stderr, "Invalid IP header length: %u bytes\n", size_ip);
        return;
    }

    // Получение UDP заголовка
    udph = (struct udphdr *)(packet + SIZE_ETHERNET + size_ip);
    size_udp = sizeof(struct udphdr);

    // Получение данных (payload) UDP пакета
    payload = packet + SIZE_ETHERNET + size_ip + size_udp;
    size_payload = ntohs(udph->len) - size_udp;

    // Вывод содержимого UDP пакета
    printf("UDP Packet captured:\n");
    printf("Source IP: %s\n", inet_ntoa(iph->ip_src));
    printf("Destination IP: %s\n", inet_ntoa(iph->ip_dst));
    printf("Source Port: %d\n", ntohs(udph->source));
    printf("Destination Port: %d\n", ntohs(udph->dest));
    printf("Payload (%d bytes):\n", size_payload);
    if (size_payload > 0) {
        printf("%.*s\n", size_payload, payload);
    }
    printf("-------------------------------------------\n");
}

int main(int argc, char *argv[]) {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    char filter_exp[] = "udp";
    struct bpf_program fp;
    bpf_u_int32 net;

    // Открытие сетевого интерфейса для захвата пакетов
    handle = pcap_open_live(argv[1], SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device %s: %s\n", argv[1], errbuf);
        return EXIT_FAILURE;
    }

    // Компиляция фильтра для захвата UDP пакетов
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Could not parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return EXIT_FAILURE;
    }

    // Применение фильтра к открытому интерфейсу
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Could not install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return EXIT_FAILURE;
    }

    // Захват пакетов и вызов обработчика для каждого захваченного пакета
    printf("Sniffing UDP packets on interface %s...\n", argv[1]);
    pcap_loop(handle, 0, packet_handler, NULL);

    // Закрытие захвата пакетов и освобождение ресурсов
    pcap_freecode(&fp);
    pcap_close(handle);

    return EXIT_SUCCESS;
}
