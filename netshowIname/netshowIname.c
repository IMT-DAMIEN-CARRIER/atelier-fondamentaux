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
    struct ifaddrs *addrs, *tmp;

    if (-1 == getifaddrs(&addrs)) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    tmp = addrs;

    printf("------------------------- LISTES DES INTERFACES RÉSEAUX -------------------------\n");

    while (tmp)
    {
        if (tmp->ifa_addr && AF_PACKET == tmp->ifa_addr->sa_family)
            printf("%s\n", tmp->ifa_name);

        tmp = tmp->ifa_next;
    }

    printf("---------------------------------------------------------------------------------\n");

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