#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <poll.h>
#include "erproc.h"


int Socket(int domain, int type, int protocol){
    int res = socket(domain, type, protocol);
    if(res == -1){
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Bind(int socket, const struct sockaddr *address, socklen_t addrlen){
    int res = bind(socket, address, addrlen);
    if(res == -1){
        perror("Bind error");
        exit(EXIT_FAILURE);
    }
}
void Listen(int socket, int backlog){
    int res = listen(socket, backlog);
    if(res == -1){
        perror("Listen error");
        exit(EXIT_FAILURE);
    }
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout){
    int res = select(nfds, readfds, writefds, exceptfds, timeout);
    if (res == -1){
        perror("Select error");
        exit(EXIT_FAILURE);
    }
    return res;
}

int Accept(int socket, struct sockaddr *address, socklen_t *addrlen){
    int res = accept(socket, address, addrlen);
    if(res == -1){
        perror("Accept error");
        exit(EXIT_FAILURE);
    }
    return res;
}

int Poll(struct pollfd fds[], nfds_t nfds, int timeout){
    int res = poll(fds, nfds, timeout);
    if(res == -1){
        perror("Poll error");
        exit(EXIT_FAILURE);
    }
    return res;
}
