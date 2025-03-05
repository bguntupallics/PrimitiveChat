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
#include <pthread.h>
#include "global.h"
#include "client_outputs.h"

volatile int heartbeat_running = 0;
pthread_t heartbeat_thread;

// Heartbeat loop function that runs in a separate thread.
// It sends a HEARTBEAT enum every 5 seconds.
void *heartbeat_loop(void *arg) {
    struct client *client = (struct client *) arg;
    enum COMMAND heartbeat = HEARTBEAT;

    while (heartbeat_running) {
        if (send(client->socketfd, &heartbeat, sizeof(heartbeat), 0) < 0) {
            perror("Heartbeat send failed");
        }

        sleep(HEARTBEAT_INTERVAL);
    }
    return NULL;
}

void disconnect(struct client *client) {
    enum COMMAND command = DISCONNECT;

    send(client->socketfd, &command, sizeof(enum COMMAND), 0);

    heartbeat_running = 0;
    pthread_join(heartbeat_thread, NULL);

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
    if(connectfd != 0) {
        printf("Connection to server failed. Going back to menu.\n");
        return;
    }
    command = CONNECT;

    send(socketfd, &command, sizeof(command), 0);
    recv(socketfd, &command, sizeof(command), 0);
    if(command != ACK) {
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

    // Start the heartbeat thread upon successful connection
    heartbeat_running = 1;
    if (pthread_create(&heartbeat_thread, NULL, heartbeat_loop, client) != 0) {
        perror("Failed to start heartbeat thread");
    }
}

void list_users(struct client *client) {
    struct list_packet list_packet;
    enum COMMAND command = LIST_USERS;

    send(client->socketfd, &command, sizeof(command), 0);
    recv(client->socketfd, &list_packet, sizeof(list_packet), 0);

    print_users(&list_packet);
}

void change_name(struct client *client) {
    struct name_packet name_packet;
    enum COMMAND command = NICKNAME;
    char new_name[20];
    int count;

    count = scanf("%s", new_name);
    if (count != 1) {
        printf("Error: 'nickname' command requires an argument.\n");
        return;
    }

    strcpy(name_packet.name, new_name);
    name_packet.name_length = strlen(new_name);

    send(client->socketfd, &command, sizeof(command), 0);

    send(client->socketfd, &name_packet, sizeof(name_packet), 0);
    recv(client->socketfd, &command, sizeof(command), 0);

    if(command == ACK) {
        successful_name_change(new_name);
    } else {
        name_already_exists(new_name);
    }
}
