//
// Created by Bhargav Guntupalli on 2/23/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/socket.h>
#include <stddef.h>
#include <netdb.h>
#include "global.h"

int init(void) {
    int socket_fd, bind_fd;
    struct sockaddr_in serveraddr;

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_fd < 0){
        perror("socket()");
    }

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(45556);
    serveraddr.sin_family = AF_INET;

    bind_fd = bind(socket_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(bind_fd != 0){
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void server_loop(struct manager *manager) {

    for(;;){

    }
}