#include <stdio.h>
#include <netinet/in.h>
#include "global.h"
#include "server_network.h"

void run(struct manager *manager, struct sockaddr_in *serveraddr) {
    manager->master_socket = init(serveraddr);

    printf("Starting Server... Press CTRL + C to Quit. \n");
    server_loop(manager, serveraddr);
}

int main() {
    struct manager manager;
    struct sockaddr_in severaddr;

    run(&manager, &severaddr);
    return 0;
}