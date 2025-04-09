//
// Created by Bhargav Guntupalli on 2/23/25.
// contains all enums and structs needed for the project
//

#ifndef PRIMITIVECHAT_GLOBAL_H
#define PRIMITIVECHAT_GLOBAL_H

#include <time.h>

#define TRUE 1
#define FALSE 0
#define ERROR (-1)
#define MAX_CLIENTS 64
#define NAME_LENGTH 20
#define MESSAGE_LENGTH 256
#define PORT 45556
#define HEARTBEAT_INTERVAL 5

// Command Enum
enum COMMAND {
    ACK = 0x01,
    CONNECT = 0x02,
    DISCONNECT = 0x03,
    HEARTBEAT = 0x04,
    LIST_USERS = 0x05,
    NICKNAME = 0x06,
    MESSAGE = 0x07,
    ECHO = 0x08,
    INVALID = 0x09,
};

struct client {
    char nickname[NAME_LENGTH];
    time_t last_heartbeat;
    int socketfd;
};

struct manager {
    int num_clients;
    int master_socket;
    struct client clients[MAX_CLIENTS];
};

struct list_packet {
    int num_users;
    int name_lengths[MAX_CLIENTS];
    char user_names[MAX_CLIENTS][NAME_LENGTH];
};

struct message_packet {
    int is_echo;
    char target_name[NAME_LENGTH];
    char sender_name[NAME_LENGTH];
    char message[MESSAGE_LENGTH];
};

struct name_packet {
    int name_length;
    char name[NAME_LENGTH];
};

#endif //PRIMITIVECHAT_GLOBAL_H
