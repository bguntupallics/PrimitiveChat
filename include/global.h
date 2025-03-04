//
// Created by Bhargav Guntupalli on 2/23/25.
// contains all enums and structs needed for the project
//

#ifndef PRIMITIVECHAT_GLOBAL_H
#define PRIMITIVECHAT_GLOBAL_H

#define TRUE 1
#define FALSE 0
#define ERROR (-1)
#define MAX_CLIENTS 64
#define PORT 45556

// Command Enum
enum COMMAND {
    CONNECT = 0x01,
    QUIT = 0x00,
    MESSAGE = 0x04,
    ECHO = 0x05,
    LIST_USERS = 0x08,
    LIST_ROOMS = 0x09,
    ALIVE = 0x0B,
    NICKNAME = 0x0C,
    INVALID = 0x0D,
    DISCONNECT = 0x0E,
    ACK = 0x0F,
};

struct name_packet {
    enum COMMAND response;
    int name_length;
    char name[256];
};

struct client {
    char nickname[256];
    double lifetime;
    int socketfd;
};

struct manager {
    int num_clients;
    int master_socket;
    struct client clients[64];
};

#endif //PRIMITIVECHAT_GLOBAL_H
