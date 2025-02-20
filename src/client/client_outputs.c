#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "client_outputs.h"

void help(void) {
    printf("help -< shows a list of commands. \n");
    printf("exit -< exits the program. \n");
}

void close(void) {
    printf("Thank You! See You Soon!\n");
    exit(0);
}

void invalid_command(void) {
    printf("Invalid Command. Please Try Again. \n");
}
