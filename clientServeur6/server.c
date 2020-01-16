//
// Created by Damien Carrier on 14/01/2020.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 30000

char buffer[1000];
int maSocket;

void fin(void)
{
    printf("\nArret du serveur.\n");
    shutdown(maSocket, 2);
    close(maSocket);
    exit(0);
}

void closeSocket(int maSocket)
{
    shutdown(maSocket, 2);
    close(maSocket);
}

void serveur(void)
{
    int client_socket;
    struct sockaddr_in monAdress, clientAddress;
    unsigned int monAddressLongueur;
    int lg;

    bzero(&monAdress, sizeof(monAdress));
    monAdress.sin_port = htons(PORT);
    monAdress.sin_family = AF_INET;
    monAdress.sin_addr.s_addr = htonl(INADDR_ANY);

    /* creation de socket */
    if ((maSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Erreur à la création\n");
        exit(0);
    }

    signal(SIGINT, fin);

    /* bind serveur - socket */
    bind(maSocket, (struct sockaddr *) &monAdress, sizeof(monAdress));

    /* ecoute sur la socket */
    listen(maSocket, 5);

    /* accept la connexion */
    monAddressLongueur = sizeof(clientAddress);

    while (1) {
        client_socket = accept(maSocket, (struct sockaddr *) &clientAddress, &monAddressLongueur);

        if (fork() == 0) {
            close(maSocket);
            lg = read(client_socket, buffer, 1000);
            printf("le serveur a recu: %s\n", buffer);
            sprintf(buffer, "%s du serveur\n", buffer);
            write(client_socket, buffer, 1000);
            fin();
        }
    }

    closeSocket(maSocket);
}

int main(void)
{
    serveur();

    return 0;
}