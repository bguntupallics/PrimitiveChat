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
#include <time.h>
#include <ctype.h>
#include "global.h"

int connect(void) {
    struct sockaddr_in server;
    enum COMMAND command;
    int connectfd, socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server.sin_addr = "127.0.0.1";
    server.sin_port = htons(45556);
    server.sin_addr = AF_INET;

    connectfd = connect(socketfd, (struct sockaddr *)&server, sizeof(server));
    if(connectfd != 0){
        printf("Connection to server failed. Going back to menu.\n");
        return -1;
    }
    command = CONNECT;

    send(socketfd, &command, sizeof(command), 0);
    recv(socketfd, &packet, sizeof(packet), 0);

    printf("Connected to Server. Your name is \"%s\"\n", packet.name);
    return socketfd;
}