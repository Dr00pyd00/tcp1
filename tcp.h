#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>

#define DEFAULT_PORT 8989
#define MAX_LISTENS 10
#define MAX_CLIENTS 50 
#define BUFFER_SIZE 512


// FUNC DECLARATIONS

int createEmptySocket(void);

int bind_socket(int socketFD, int port );

int listen_socket(int socketFD);

int recvMessageFromSocket(int socketFD, char *buffer, size_t bufferSize);

