#include <stdio.h>
#include <stdlib.h>
#include "client_outputs.h"

void disconnected_help(void) {
    printf("help <- shows a list of commands. \n");
    printf("exit <- exits the program. \n");
    printf("connect <- connects to the server. \n");
}

void connected_help(void) {

}

void close(void) {
    printf("Thank You! See You Soon!\n");
    exit(0);
}

void invalid_command(void) {
    printf("Invalid Command. Please Try Again. \n");
}
