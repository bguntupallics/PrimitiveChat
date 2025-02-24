//
// Created by Bhargav Guntupalli on 2/23/25.
// contains all enums and structs needed for the project
//

#ifndef PRIMITIVECHAT_GLOBAL_H
#define PRIMITIVECHAT_GLOBAL_H

// Command Enum
enum COMMAND {
    CONNECT = 0x01,
    LEAVE_ROOM = 0x03,
    JOIN_ROOM = 0x02,
    QUIT = 0x00,
    MESSAGE = 0x04,
    ECHO = 0x05,
    KICK = 0x07,
    TALK = 0x06,
    LIST_USERS = 0x08,
    LIST_ROOMS = 0x09,
    HELP = 0x0A,
    ALIVE = 0x0B,
    NICKNAME = 0x0C,
    INVALID = 0x0D,
    DISCONNECT = 0x0E,
    ACK = 0x0F,
    ERROR = 0x10
};

struct name_packet {
    enum COMMAND response;
    int name_length;
    char name[256];
};

#endif //PRIMITIVECHAT_GLOBAL_H
