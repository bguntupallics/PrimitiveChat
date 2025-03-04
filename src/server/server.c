#include <stdio.h>
#include <netinet/in.h>
#include "global.h"
#include "server_network.h"
#include "server_outputs.h"

void run(struct manager *manager, struct sockaddr_in *serveraddr) {
    init(manager, serveraddr);

    print_init();
    server_loop(manager, serveraddr);
}

int main(void) {
    struct manager manager;
    struct sockaddr_in severaddr;

    run(&manager, &severaddr);
    return 0;
}
