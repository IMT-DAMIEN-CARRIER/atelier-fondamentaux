//
// Created by Damien Carrier on 14/01/2020.
//

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVEUR_NAME "A RENSEIGNER" //IP VYOS
#define PORT 30000

int to_server_socket = -1;

/**
 * Méthode principale permettant de jouer la fonction de client.
 */
void client(void)
{
    char *server_name = SERVEUR_NAME;
    struct sockaddr_in serverSockAddr;
    struct hostent *serverHostEnt;
    long hostAddr;
    long status;
    char buffer[512];
    char message[1000];
    char ligne[100];
    char ligneaddr[150];

    bzero(&serverSockAddr, sizeof(serverSockAddr));
    hostAddr = inet_addr(SERVEUR_NAME);

    if ((long) -1 != (long) hostAddr) {
        bcopy(&hostAddr, &serverSockAddr.sin_addr, sizeof(hostAddr));
    } else {
        serverHostEnt = gethostbyname(SERVEUR_NAME);

        if (serverHostEnt == NULL) {
            printf("Erreur à gethostbyname\n");
            exit(0);
        }

        bcopy(serverHostEnt->h_addr, &serverSockAddr.sin_addr, serverHostEnt->h_length);
    }

    serverSockAddr.sin_port = htons(PORT);
    serverSockAddr.sin_family = AF_INET;

    /* creation de la socket */
    if ((to_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Erreur à la création socket client\n");
        exit(0);
    }

    /* requete de connexion */
    if (connect(to_server_socket, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) < 0) {
        printf("Erreur à la demande de connection\n");
        exit(0);
    }

    char cmd[200];

    system("netshow > ~/tmp.txt");
    FILE *file = popen("cat ~/tmp.txt", "r");

    if (file != NULL) {
        while (fgets(ligne, 100, file) != NULL) {
            sprintf(cmd, "netshow %s\n", ligne);
            FILE *addresses = popen(cmd, "r");

            while (fgets(ligneaddr, 150, addresses) != NULL) {
                strcat(message, ligneaddr);
            }
        }
        write(to_server_socket, message, strlen(message));
        read(to_server_socket, buffer, 512);
        printf("%s\n", buffer);
        system("rm ~/tmp.txt");
    }

    /* fermeture de la connection */
    shutdown(to_server_socket, 2);
    close(to_server_socket);
}

int main(void)
{
    client();

    return 0;
}