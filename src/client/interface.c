#include <stdio.h>
#include <string.h>
#include <string.h>
#include "interface.h"
#include "client_outputs.h"
#define TRUE 1
#define FALSE 0

void parse_command(char *command){
    if(strcmp(command, "exit") == 0){
        close();
    } else if(strcmp(command, "help") == 0){
        help();
    } else {
        invalid_command();
    }
}

void run(void) {
    char command[2048];

    printf("Welcome to Primitive Chat! Type \"help\" for a list of commands ->\n");

    for(;;){
        scanf("%s", command);

        parse_command(command);

        memset(&command, 0, strlen(command));
    }
}
