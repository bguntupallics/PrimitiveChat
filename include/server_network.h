//
// Created by Bhargav Guntupalli on 2/23/25.
//

#ifndef PRIMITIVECHAT_SERVER_NETWORK_H
#define PRIMITIVECHAT_SERVER_NETWORK_H

#include "global.h"

int init(struct sockaddr_in *serveraddr);

void accept_client(struct manager *manager, struct sockaddr_in *serveraddr);

void process_client_requests(struct manager *manager,fd_set readfds);

void server_loop(struct manager *manager, struct sockaddr_in *serveraddr);

#endif //PRIMITIVECHAT_SERVER_NETWORK_H
