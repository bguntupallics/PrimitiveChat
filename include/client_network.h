//
// Created by Bhargav Guntupalli on 2/23/25.
//

#ifndef PRIMITIVECHAT_CLIENT_NETWORK_H
#define PRIMITIVECHAT_CLIENT_NETWORK_H

#include "global.h"

void disconnect(struct client *client);

int connect_to_server(struct client *client);

#endif //PRIMITIVECHAT_CLIENT_NETWORK_H
