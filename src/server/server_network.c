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
#include "server_outputs.h"

void init(struct manager *manager, struct sockaddr_in *serveraddr) {
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

    //initializing manager struct
    manager->num_clients = 0;
    manager->master_socket = socket_fd;
}

int assign_number(struct manager *manager) {
    int user_number = 1;
    int found;
    int client_num;

    while (1) {
        found = 0;
        // Check each client to see if they already have the name "user<user_number>"
        for (int i = 0; i < manager->num_clients; i++) {
            // Try to extract a number from the client's name (assuming the pattern "user%d")
            if (sscanf(manager->clients[i].nickname, "user%d", &client_num) == 1) {
                if (client_num == user_number) {
                    found = 1;
                    break;
                }
            }
        }
        // If no client has the current number, return it
        if (!found) {
            return user_number;
        }
        user_number++;
    }
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

    //sending welcome packet with their initial username back to the client
    snprintf(username, sizeof(username), "user%d", assign_number(manager));
    strncpy(welcome_packet.name, username, NAME_LENGTH);
    welcome_packet.name_length = strlen(username);
    send(new_socket, &welcome_packet, sizeof(welcome_packet), 0);

    //modifying manager data structure
    strncpy(manager->clients[manager->num_clients].nickname, username, NAME_LENGTH);
    manager->clients[manager->num_clients].socketfd = new_socket;
    manager->clients[manager->num_clients].last_heartbeat = time(NULL);
    manager->num_clients = manager->num_clients + 1;

    print_new_client(username);
    print_manager_struct(manager);
}

void remove_client(struct manager *manager, int index) {
    // Check that the index is valid.
    if (index < 0 || index >= manager->num_clients) {
        fprintf(stderr, "remove_client: index %d out of range (num_clients=%d)\n",
                index, manager->num_clients);
        return;
    }

    //closing the connection with the client
    close(manager->clients[index].socketfd);

    // Shift clients after the removed index one position to the left.
    for (int i = index; i < manager->num_clients - 1; i++) {
        manager->clients[i] = manager->clients[i + 1];
    }

    // Optionally, clear the last client (not strictly necessary)
    memset(&manager->clients[manager->num_clients - 1], 0, sizeof(struct client));

    // Decrement the number of active clients.
    manager->num_clients--;
    print_manager_struct(manager);
}

void send_list(struct manager *manager, int index) {
    struct list_packet toSend;
    int i;

    toSend.num_users = manager->num_clients;
    for(i = 0; i < manager->num_clients; i++) {
        toSend.name_lengths[i] = strlen(manager->clients[i].nickname);
        strncpy(toSend.user_names[i], manager->clients[i].nickname, NAME_LENGTH);
    }

    send(manager->clients[index].socketfd, &toSend, sizeof(toSend), 0);
}

//change name only if the name is not taken
void change_name(struct manager *manager, int index) {
    enum COMMAND to_return;
    struct name_packet name_packet;
    int i, exists = 0;

    recv(manager->clients[index].socketfd, &name_packet, sizeof(name_packet), 0);

    //first check if name is taken
    for(i = 0; i < manager->num_clients; i++) {
        if(strcmp(manager->clients[i].nickname, name_packet.name) == 0) {
            exists = 1;
            break;
        }
    }

    if(exists) {
        to_return = INVALID;
        send(manager->clients[index].socketfd, &to_return, sizeof(to_return), 0);
    } else {
        strcpy(manager->clients[index].nickname, name_packet.name);

        to_return = ACK;
        send(manager->clients[index].socketfd, &to_return, sizeof(to_return), 0);
    }
}

void process_message(struct manager *manager, int index) {
    enum COMMAND command;
    struct message_packet message_packet;
    struct client client = manager->clients[index];
    int received, sent, target_index = -1, i;

    received = (int) recv(client.socketfd, &message_packet, sizeof(message_packet), 0);
    if(received < 0) {
        command = ERROR;
        send(client.socketfd, &command, sizeof(command), 0);
        perror("Error receiving message packet from client. \n");
        return;
    }

    for(i = 0; i < manager->num_clients; i++) {
        if(strcmp(manager->clients[i].nickname, message_packet.target_name) == 0) {
            target_index = i;
            break;
        }
    }

    if(target_index < 0) {
        command = INVALID;
        send(client.socketfd, &command, sizeof(command), 0);
        return;
    } else {
        sent = (int) send(manager->clients[target_index].socketfd, &message_packet, sizeof(message_packet), 0);

        if(sent < 0) {
            command = ERROR;
            send(client.socketfd, &command, sizeof(command), 0);
            perror("Error sending message packet to target. \n");
            return;
        }

        command = ACK;
        send(client.socketfd, &command, sizeof(command), 0);
    }
}

void update_heartbeat(struct manager *manager, int index) {
    manager->clients[index].last_heartbeat = time(NULL);
    print_heartbeat(manager->clients[index].nickname);
}

void process_client_request(struct manager *manager, int index) {
    enum COMMAND command;
    struct client client = manager->clients[index];
    int received;
    
    received = (int) recv(client.socketfd, &command, sizeof(command), 0);
    if(received < 0){
        perror("Error receiving command from client");
        // figure out how to handle this later
    }

    switch(command){
        case DISCONNECT:
            remove_client(manager, index);
            break;
        case HEARTBEAT:
            update_heartbeat(manager, index);
            break;
        case LIST_USERS:
            send_list(manager, index);
            break;
        case NICKNAME:
            change_name(manager, index);
            break;
        case MESSAGE:
            process_message(manager, index);
        default:
            unexpected_command(manager->clients[index].nickname);
            break;
    }
}

// Check all clients for heartbeat timeouts.
void check_client_timeouts(struct manager *manager) {
    time_t now = time(NULL);
    int i;

    for (i = 0; i < manager->num_clients; i++) {
        if (difftime(now, manager->clients[i].last_heartbeat) > (HEARTBEAT_INTERVAL * 2)) {
            printf("Client \"%s\" timed out.\n", manager->clients[i].nickname);
            remove_client(manager, i);
            i--; // Adjust index because clients have shifted
        }
    }
}

void server_loop(struct manager *manager, struct sockaddr_in *serveraddr) {
    fd_set readfds;
    int select_result, maxfd, i;

    if (listen(manager->master_socket, MAX_CLIENTS) < 0) {
        perror("listen failed");
        close(manager->master_socket);
        exit(EXIT_FAILURE);
    }

    for(;;) {
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

        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        select_result = select(maxfd + 1, &readfds, NULL, NULL, &tv);
        if(select_result < 0){
            perror("select()");
        }

        if(FD_ISSET(manager->master_socket, &readfds)){ //new connection incoming
            accept_client(manager, serveraddr);
        }
        
        for(i = 0; i < manager->num_clients; i++){
            if(FD_ISSET(manager->clients[i].socketfd, &readfds)){
                process_client_request(manager, i);
            }
        }

        check_client_timeouts(manager);
    }
}
