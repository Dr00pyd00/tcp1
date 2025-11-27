#include "tcp.h"


//===============================
//====== Func implementations
//===============================

//=========================================================================

// CREATE SOCKET

// return FD socket 
int createEmptySocket(void) {
    return socket(AF_INET, SOCK_STREAM, 0);
}

//=========================================================================

// BIND SOCKET

//On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error
int bind_socket(int socketFD, int port ) {
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons((port) ? port : DEFAULT_PORT );
    serverAddr.sin_family = AF_INET;

    return bind(socketFD, (const struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));
}

//=========================================================================

// LISTEN 

//On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error.
int listen_socket(int socketFD) {
    return listen(socketFD, MAX_LISTENS);
}

//=========================================================================

// RECV MESSAGE

//On success num of octets recv returned, if 0 = Client leaved, On error, -1 is returned
int recvMessageFromSocket(int socketFD, char *buffer, size_t bufferSize) {

    int n = recv(socketFD, buffer, bufferSize, 0);
    // end of buffer:
    if ( n > 0) buffer[n] = '\0';
    // returns:
    if ( n < 0) {
        return -1;
    }
    else if ( n == 0) {
        return 0;
    }
    else {
        return n;
    }
}   


//=========================================================================

