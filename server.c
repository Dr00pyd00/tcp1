#include "tcp.h"





// variables globale pour pouvoir utiliser handle_signal...
int serverSocketFD; 
int allClientsFD[MAX_CLIENTS];


// func for leave: 
void handle_signal(int sig) {
    printf("Caught signal %d, closing all sockets...\n", sig);
    // fermer tout les clients:
    for (int i = 0; i < MAX_CLIENTS; i ++) {
        if (allClientsFD[i] > 0) close(allClientsFD[i] );
    }
    if (serverSocketFD > 0) close(serverSocketFD);

    _exit(0);
}




int main(int argc, char **argv) {
    (void)argc;
    (void)argv;


    //=========== quitter proprement:
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // tableau contenant les fds de clients pour le select : tout a -1 si vide
   
    for (int i = 0; i < MAX_CLIENTS; i++) 
        allClientsFD[i] = -1;


    // create bind listen (serverr)
    serverSocketFD = createEmptySocket();
    if (serverSocketFD < 0) { perror("socket"); return 1;}
    if (bind_socket(serverSocketFD, port ) < 0) { perror("bind"); return 2;}
    if ( listen_socket(serverSocketFD) < 0) { perror("listen"); return 3;}

    printf("Server listening on port %d...\n", port);

    //=========================================================
    // fd_set : contenant des fd pouvant etre surveiller par select
    fd_set readfds;
    int maxFD = serverSocketFD;


    //============================================================
    // DEBUT BOUCLE

    while (1) {
        FD_ZERO(&readfds); // memset like
        FD_SET(serverSocketFD, &readfds); // ajout du serversocket dans le set de base

        // ajouter les clients dans le set:
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (allClientsFD[i] > 0) {
                FD_SET(allClientsFD[i], &readfds);
            }
        }

        // lancement du select:
        int activity = select(maxFD +1, &readfds, NULL, NULL, NULL);
        if (activity < 0) { perror("select"); break;}

        // nouveau client:
        if (FD_ISSET(serverSocketFD, &readfds)) {
            int newClientFD = accept(serverSocketFD, (struct sockaddr *)&clientAddr, &clientAddrSize );
            if (newClientFD < 0) { perror("accept"); continue; } 
            printf("New client accepeted: FD=%d\n", newClientFD);
       

            // ajouter le nouveau client a la liste:
            int added = 0;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (allClientsFD[i] == -1) {
                    allClientsFD[i] = newClientFD;
                    if ( newClientFD > maxFD ) maxFD = newClientFD;
                    added = 1;
                    break;
                }
            }
            if (!added) {
                printf("Max client reached. CLosing new connection...\n");
                close(newClientFD);
            }
        }

        // lire les clients existants :
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = allClientsFD[i];
            if ( fd > 0 && FD_ISSET(fd, &readfds)) {
                int n = recv(fd, buffer, BUFFER_SIZE-1, 0);
                if (n <= 0) {
                    printf("client FD=%d disconnected...\n", fd);
                    close(fd);
                    allClientsFD[i] = -1;
                } else {
                    buffer[n] = '\0';
                    printf("client FD=%d send: %s\n", fd, buffer);
                }
            }
        }

}//fin while

// fermer toutes les sockets:
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (allClientsFD[i] > 0) close(allClientsFD[i]);
    }
    close(serverSocketFD);


    return 0;
}