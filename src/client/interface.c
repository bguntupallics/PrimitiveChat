#include <stdio.h>
#include <string.h>
#include "interface.h"
#include <stdlib.h>
#include "global.h"
#include "client_outputs.h"
#include "client_network.h"

uint8_t connected = FALSE;
struct client client;

void sigint_handler(int signum) {
    (void)signum; // avoid unused parameter warning

    if (connected) {
        disconnect(&client);
    }
    close_program();
}

void parse_command(char *command){
    if(strcmp(command, "exit") == 0){
        if(connected){
            disconnect(&client);
        }

        close_program();
    } else if(strcmp(command, "help") == 0){
        help(connected);
    } else if(strcmp(command, "connect") == 0){
        connect_to_server(&client, &connected);
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

    // Install the SIGINT handler (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error installing SIGINT handler");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to Primitive Chat! Type \"help\" for a list of commands ->\n");

    for(;;){
        printf("~ ");
        scanf("%s", command);

        parse_command(command);

        memset(&command, 0, strlen(command));
    }
}
