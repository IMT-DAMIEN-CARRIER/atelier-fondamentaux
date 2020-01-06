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

int nethowIp(char *ifname[])
{
    struct ifaddrs *list;
    int res = getifaddrs(&list);
    char host[NI_MAXHOST];

    if (0 == res) {
        printf("Vous voulez des informations sur : %s\n", ifname[1]);

        while (list) {
            if (!strcmp(list->ifa_name, ifname[1])) {
                struct sockaddr *sockaddr = list->ifa_addr;

                if (AF_INET == sockaddr->sa_family || AF_INET6 == sockaddr->sa_family) {
                    int s = getnameinfo(
                            sockaddr,
                            (AF_INET == sockaddr->sa_family) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                            host,
                            NI_MAXHOST,
                            NULL,
                            0,
                            NI_NUMERICHOST
                     );

                    if (0 != s) {
                        printf("getnameinfo failed\n");
                    } else {
                        switch (sockaddr->sa_family) {
                            case AF_INET:
                                printf("adresse IPv4: %s\n", host);
                                break;
                            case AF_INET6:
                                printf("adresse IPv6: %s\n", host);
                                break;
                        }
                    }
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