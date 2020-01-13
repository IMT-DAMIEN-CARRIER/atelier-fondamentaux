#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <linux/if_link.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <string.h>

static int bitCount(uint32_t i) {
    int count = 0;

    while (i > 0) {
        if (1 & i) {
            count++;
        }

        i >>= 1;
    }

    return count;
}

int nethowIp(char *ifname[])
{
    struct ifaddrs *list;
    int res = getifaddrs(&list);
    char host[NI_MAXHOST];
    char ipstrv6[50];
    char addr[50];
    char mask[50];

    if (0 == res) {
        printf("Vous voulez des informations sur : %s\n\n", ifname[1]);

        while (list) {
            if (!strcmp(list->ifa_name, ifname[1])) {
                if (AF_INET == list->ifa_addr->sa_family) {
                    struct sockaddr_in *in = (struct sockaddr_in *) list->ifa_addr;
                    inet_ntop(AF_INET, &in->sin_addr, addr, sizeof(addr));

                    struct sockaddr_in *mask4 = (struct sockaddr_in *) list->ifa_netmask;
                    inet_ntop(AF_INET, &mask4->sin_addr, mask, sizeof(addr));

                    int n;
                    inet_pton(AF_INET, mask, &n);
                    int i = 0;

                    while (n > 0) {
                        n = n >> 1;
                        i++;
                    }

                    printf("\t- adresse IPv4: %s\n", addr);
                    printf("\t- masque réseau (IPv4) : /%d\n", i);
                } else if (AF_INET6 == list->ifa_addr->sa_family) {
                    const struct in6_addr *addr = &((struct sockaddr_in6 *) list->ifa_addr)->sin6_addr;
                    char ipstrv6[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET6, addr, ipstrv6, sizeof(ipstrv6));

                    const void *netaddr = &((struct sockaddr_in6 *) list->ifa_netmask)->sin6_addr;
                    struct in6_addr netmaskv6 = (((struct sockaddr_in6 *) list->ifa_netmask)->sin6_addr);
                    int cidrv6 = 0;

                    for (int byte = 0; byte < 16; byte++) {
                        int temp = bitCount(netmaskv6.s6_addr[byte]);
                        cidrv6 = cidrv6 + temp;
                    }

                    printf("\t- adresse IPv6: %s\n", ipstrv6);
                    printf("\t- masque réseau (IPv6) : /%d\n", cidrv6);
                }

            }

            list = list->ifa_next;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *ifname[])
{
    if (1 >= argc || 2 < argc) {
        printf("Le nombre de paramètres est incorrect..\n");
        return EXIT_FAILURE;
    }

    if (2 == argc) {
        nethowIp(ifname);
    }

    return 0;
}