#include <netinet/in.h>
#include "global.h"
#include "server_network.h"
#include "server_outputs.h"

void run(void) {
    struct manager manager;
    struct sockaddr_in serveraddr;

    init(&manager, &serveraddr);

    print_init();
    server_loop(&manager, &serveraddr);
}

int main(void) {
    run();
    return 0;
}
