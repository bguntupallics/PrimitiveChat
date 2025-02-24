#include <stdio.h>
#include <string.h>
#include <string.h>
#include "interface.h"
#include <stdint.h>
#include "client_outputs.h"
#include "global.h"
#include "client_network.h"

#define TRUE 1
#define FALSE 0

uint8_t connected = FALSE;

void parse_command(char *command){
    if(strcmp(command, "exit") == 0){
        close();
    } else if(strcmp(command, "help") == 0){
        (connected ? connected_help() : disconnected_help());
    } else if(strcmp(command, "connect")){
        connect();
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
