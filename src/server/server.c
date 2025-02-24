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
#include "server_network.h"

struct manager manager;

void run(int socket_fd) {
    manager.master_socket = socket_fd;

    printf("Starting Server... Press CTRL + C to Quit. \n");
    server_loop(&manager);
}

int main() {
    run(init());
    return 0;
}