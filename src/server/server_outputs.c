//
// Created by Bhargav Guntupalli on 2/19/25.
//

#include <stdio.h>
#include "global.h"

void print_init(void) {
    printf("Starting Server... Press CTRL + C to Quit. \n");
}

void print_new_client(const char *name) {
    printf("New Client Connected. Name -> %s. \n", name);
}

void print_manager_struct(struct manager *manager) {
    int i;

    printf("Master Socket: %d \n", manager->master_socket);
    printf("Number of Clients: %d \n", manager->num_clients);

    for(i = 0; i < manager->num_clients; i++){
        printf("Name: %s, Socket: %d \n", manager->clients[i].nickname, manager->clients[i].socketfd);
    }
}
