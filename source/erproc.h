#ifndef ERPROC_H
#define ERPROC_H

#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include<poll.h>

int Socket(int domain, int type, int protocol);

void Bind(int socket, const struct sockaddr *address, socklen_t addrlen);

void Listen(int socket, int backlog);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

int Accept(int socket, struct sockaddr *address, socklen_t *addrlen);

int Poll(struct pollfd fds[], nfds_t nfds, int timeout);

#endif
