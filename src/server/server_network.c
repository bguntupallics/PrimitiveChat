//
// Created by Bhargav Guntupalli on 2/23/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <stdint.h>
#include <sys/socket.h>
#include <stddef.h>
//#include <netdb.h>
#include "global.h"

int init(struct sockaddr_in *serveraddr) {
    int socket_fd, bind_fd;

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_fd < 0){
        perror("socket()");
    }

    serveraddr->sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr->sin_port = htons(PORT);
    serveraddr->sin_family = AF_INET;

    bind_fd = bind(socket_fd, (const struct sockaddr *) serveraddr, sizeof(struct sockaddr_in));
    if(bind_fd != 0){
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void accept_client(struct manager *manager, struct sockaddr_in *serveraddr) {
    int new_socket, received, len;
    struct name_packet welcome_packet;
    enum COMMAND command;
    char username[10];

    new_socket = accept(manager->master_socket, (struct sockaddr *)&serveraddr, (socklen_t *)&len);
    if(new_socket < 0){
        perror("Line 601: accept()");
    }

    received = (int) recv(new_socket, &command, sizeof(command), 0);

    if(received < 0 || command != CONNECT){
        printf("Error Receiving or incorrect connect message received. Closing connection...\n");
        close(new_socket);
    }

    memset(&command, 0, sizeof(command));
    command = ACK;
    send(new_socket, &command, sizeof(command), 0);

    welcome_packet.response = NICKNAME;
    snprintf(username, sizeof(username), "user%d", manager->num_clients + 1);
    strcpy(welcome_packet.name, username);
    welcome_packet.name_length = strlen(username);
    send(new_socket, &welcome_packet, sizeof(welcome_packet), 0);
}

void process_client_requests(struct manager *manager,fd_set readfds) {

}

void server_loop(struct manager *manager, struct sockaddr_in *serveraddr) {
    fd_set readfds;
    int select_result, maxfd, i;

    if (listen(manager->master_socket, MAX_CLIENTS) < 0) {
        perror("listen failed");
        close(manager->master_socket);
        exit(EXIT_FAILURE);
    }

    for(;;){
        FD_ZERO(&readfds);

        maxfd = manager->master_socket;
        FD_SET(manager->master_socket, &readfds);

        // adding client socketfds to readfds, only occurs if at least 1 client is connected
        if(manager->num_clients > 0){
            for(i = 0; i < manager->num_clients; i++){
                if(manager->clients[i].socketfd > 0){
                    FD_SET(manager->clients[i].socketfd, &readfds);

                    if(manager->clients[i].socketfd > maxfd){
                        maxfd = manager->clients[i].socketfd;
                    }
                }
            }
        }

        select_result = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if(select_result < 0){
            perror("select()");
        }

        if(FD_ISSET(manager->master_socket, &readfds)){ //new connection incoming
            accept_client(manager, serveraddr);
        }
    }
}
