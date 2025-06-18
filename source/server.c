#include<sys/socket.h>
#include<sys/poll.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>

#include "erproc.h"

#define PORT 1488
#define MAX_CLIENTS 1023

volatile sig_atomic_t stop_server=0;

void* stop_listener(void* arg){
    char cmd[100];
    while(fgets(cmd, sizeof(cmd), stdin)){
        if(strncmp(cmd, "stop", 4)==0){
            stop_server=1;
            printf("Server is manually stoped by the 'stop' command.\n");
            break;
        }
    }

    return NULL;
}

int main(){
    int listener = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr;
    struct pollfd fds[MAX_CLIENTS+1];
    struct sockaddr_in client_adr;
    int nfds=1;
    int new_fd, nread, poll_count;
    char buf[1024];
    char *client_ip;

    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = INADDR_ANY;

    Bind(listener, (struct sockaddr *)&adr, sizeof(adr));
    Listen(listener, 8);

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    fds[0].fd = listener;
    fds[0].events = POLLIN;

    nfds=2;

    printf("Server is listening on port %d\n", PORT);
    printf("Print 'stop' to stop server. \n");


    pthread_t stop_threat;
    pthread_create(&stop_threat, NULL, stop_listener, NULL);

    while(!stop_server){
        poll_count = Poll(fds, nfds, 100);

        if(poll_count==-1){
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        if(poll_count==0){
            usleep(1000);
            continue;
        }

        for(int i=0; i<nfds; i++){
            if(fds[i].revents & POLLIN){
                if(fds[i].fd == listener){
                    socklen_t client_len = sizeof(client_adr);
                    new_fd = Accept(listener, (struct sockaddr *)&client_adr, &client_len);
                    if(nfds < MAX_CLIENTS + 2){
                        fds[nfds].fd = new_fd;
                        fds[nfds].events = POLLIN;
                        nfds++;
                        client_ip = inet_ntoa(client_adr.sin_addr);
                        printf("New client connected: %d, address: %s\n", new_fd, client_ip);
                        if(strncmp(buf, "stop", 4)==0){
                            close(new_fd);
                        }
                    } else {
                        printf("Client limit is occured.");
                        close(new_fd);
                    }
                } else {
                    memset(buf, 0, sizeof(buf));
                    nread = read(fds[i].fd, buf, sizeof(buf));
                    if(nread<=0){
                        printf("Client %s:%d disconnected.\n", client_ip, fds[i].fd);
                        close(fds[i].fd);
                        fds[i] = fds[nfds -1];
                        nfds--;
                        i--;
                    } else {
                        for(int j=2; j<nfds; j++){
                            if(j != i){
                                send(fds[j].fd, buf, nread, 0);
                            }
                        }
                    }
                }
            }
        }
    }


    return 0;
}
