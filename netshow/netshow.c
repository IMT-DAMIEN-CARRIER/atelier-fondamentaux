/**
 * Created by dcarrier on 05/11/2019.
*/

/**
 * Created by dcarrier on 05/11/2019.
*/

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

int nethowIp(char ifname[])
{
    struct ifaddrs *list;
    int res = getifaddrs(&list);
    char host[NI_MAXHOST];

    if (0 == res) {
        printf("Information sur : %s\n", ifname);

        while (list) {
            if (!strcmp(list->ifa_name, ifname)) {
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

/**
 * .
 */
int netshow()
{
    struct ifaddrs *addrs, *tmp;

    if (-1 == getifaddrs(&addrs)) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    tmp = addrs;

    printf("------------------------- LISTES DES INTERFACES RÉSEAUX -------------------------\n\n");

    while (tmp)
    {
        if (tmp->ifa_addr && AF_PACKET == tmp->ifa_addr->sa_family) {
            printf("**********************************\n");
            nethowIp(tmp->ifa_name);
            printf("**********************************\n\n");
        }

        tmp = tmp->ifa_next;
    }

    printf("---------------------------------------------------------------------------------\n\n");

    freeifaddrs(addrs);
    exit(EXIT_SUCCESS);
}

/**
 * main function.
 */
int main(void)
{
    netshow();
    return 0;
}