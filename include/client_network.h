//
// Created by Bhargav Guntupalli on 2/23/25.
//

#ifndef PRIMITIVECHAT_CLIENT_NETWORK_H
#define PRIMITIVECHAT_CLIENT_NETWORK_H

#include "global.h"

void disconnect(struct client *client);

void connect_to_server(struct client *client, uint8_t *connected);

void list_users(struct client *client);

void change_name(struct client *client);

void send_message(struct client *client, int is_echo);

void receive_message(struct  client *client);

#endif //PRIMITIVECHAT_CLIENT_NETWORK_H
