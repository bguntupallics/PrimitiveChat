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

#define MAX_CLIENTS 64

struct manager manager;

void run(int socket_fd) {
    manager.master_socket = socket_fd;
}

int main() {
    run(init());
    return 0;
}