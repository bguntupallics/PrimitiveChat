//
// Created by Bhargav Guntupalli on 2/23/25.
//

#ifndef PRIMITIVECHAT_SERVER_NETWORK_H
#define PRIMITIVECHAT_SERVER_NETWORK_H

#include "global.h"

void init(struct manager *manager, struct sockaddr_in *serveraddr);

void accept_client(struct manager *manager, struct sockaddr_in *serveraddr);

void remove_client(struct manager *manager, int index);

void process_client_request(struct manager *manager, int index);

void server_loop(struct manager *manager, struct sockaddr_in *serveraddr);

#endif //PRIMITIVECHAT_SERVER_NETWORK_H
