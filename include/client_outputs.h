//
// Created by Bhargav Guntupalli on 2/19/25.
//

#ifndef PRIMITIVECHAT_CLIENT_OUTPUTS_H
#define PRIMITIVECHAT_CLIENT_OUTPUTS_H

#include "global.h"

void help(int connected);

void cant_leave_when_not_connected(void);

void close_program(void);

void invalid_command(void);

void disconnected_from_server(void);

void successful_name_change(const char *name);

void name_already_exists(const char *name);

void print_users(struct list_packet *listPacket);

void print_message_send_success(void);

void print_message_send_failure(void);

void print_user_does_not_exist(const char *name);

void print_message(struct message_packet message_packet);

#endif //PRIMITIVECHAT_CLIENT_OUTPUTS_H
