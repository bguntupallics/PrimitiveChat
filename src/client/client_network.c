//
// Created by Bhargav Guntupalli on 2/23/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "global.h"
#include "client_outputs.h"

void disconnect(struct client *client) {
    enum COMMAND command = DISCONNECT;
    send(client->socketfd, &command, sizeof(enum COMMAND), 0);
    close(client->socketfd);
    memset(client, 0, sizeof(struct client));
    disconnected_from_server();
}

void connect_to_server(struct client *client, uint8_t *connected) {
    struct sockaddr_in server;
    enum COMMAND command;
    struct name_packet welcome_packet;
    int connectfd, socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    server.sin_port = htons(PORT);
    server.sin_family = AF_INET;

    connectfd = connect(socketfd, (struct sockaddr *)&server, sizeof(server));
    if(connectfd != 0){
        printf("Connection to server failed. Going back to menu.\n");
        return;
    }
    command = CONNECT;

    send(socketfd, &command, sizeof(command), 0);
    recv(socketfd, &command, sizeof(command), 0);
    if(command != ACK){
        perror("Error connecting to server. Closing connection. \n");
        close(socketfd);
        return;
    } else {
        recv(socketfd, &welcome_packet, sizeof(welcome_packet), 0);
    }

    strcpy(client->nickname, welcome_packet.name);
    printf("Connected to Server. Your name is \"%s\"\n", welcome_packet.name);
    *connected = TRUE;
    client->socketfd = socketfd;
}
