/**
 * Created by dcarrier on 05/11/2019.
*/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

/**
 * Methode permettant d'afficher la liste des interfaces réseaux.
 */
int netshowIname()
{
    struct ifaddrs *addrs,*tmp;

    if (getifaddrs(&addrs) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    tmp = addrs;

    printf("------------------------LISTES DES INTERFACES RÉSEAUX ----------------\n");

    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
            printf("%s\n", tmp->ifa_name);

        tmp = tmp->ifa_next;
    }

    printf("----------------------------------------------------------------------\n");

    freeifaddrs(addrs);
    exit(EXIT_SUCCESS);
}

/**
 * main function.
 */
int main(void)
{
    netshowIname();
    return 0;
}