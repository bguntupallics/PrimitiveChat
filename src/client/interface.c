#include <stdio.h>
#include <string.h>
#include <string.h>
#include "interface.h"
#include <stdint.h>
#include "global.h"
#include "client_outputs.h"
#include "client_network.h"

uint8_t connected = FALSE;
struct client client;

void parse_command(char *command){
    if(strcmp(command, "exit") == 0){
        if(connected){
            disconnect(&client);
        }

        close();
    } else if(strcmp(command, "help") == 0){
        help(connected);
    } else if(strcmp(command, "connect") == 0){
        client.socketfd = connect_to_server(&client);
    } else if(strcmp(command, "leave") == 0){
        if(connected){
            disconnect(&client);
        } else {
            cant_leave_when_not_connected();
        }
    }
    else {
        invalid_command();
    }
}

void run(void) {
    char command[2048];

    printf("Welcome to Primitive Chat! Type \"help\" for a list of commands ->\n");

    for(;;){
        printf("~ ");
        scanf("%s", command);

        parse_command(command);

        memset(&command, 0, strlen(command));
    }
}
