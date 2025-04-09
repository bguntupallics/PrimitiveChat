//
// Created by Bhargav Guntupalli on 2/19/25.
//

#ifndef PRIMITIVECHAT_SERVER_OUTPUTS_H
#define PRIMITIVECHAT_SERVER_OUTPUTS_H

void print_init(void);

void print_new_client(const char *name);

void print_manager_struct(struct manager *manager);

void print_heartbeat(const char *name);

void unexpected_command(const char *name);

#endif //PRIMITIVECHAT_SERVER_OUTPUTS_H
