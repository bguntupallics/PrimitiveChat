#include <stdio.h>
#include <stdlib.h>
#include "client_outputs.h"
#include "global.h"

void help(int connected) {
    printf("help <- shows a list of commands. \n");
    printf("exit <- exits the application. \n");

    if(connected){
        printf("leave <- disconnects from the server. \n");
        printf("message <name> <message> <- sends a message to the user with the provided name. \n");
        printf("nickname <name> <- Changes your name to the name provided (Limit 256 characters). \n");
        printf("list <- lists the other active users connected. \n");
    } else {
        printf("connect <- connects to the server. \n");
    }
}

void cant_leave_when_not_connected(void) {
    printf("Not connected to a server. Type \"exit\" if you would like to close the application. \n");
}

void close_program(void) {
    printf("Thank You! See You Soon!\n");
    exit(0);
}

void invalid_command(void) {
    printf("Invalid Command. Please Try Again. \n");
}

void disconnected_from_server(void) {
    printf("Disconnected from server. \n");
}

void print_users(struct list_packet *listPacket) {
    int i;

    printf("%d Active Users \nUsers: \n", listPacket->num_users);
    for(i = 0; i < listPacket->num_users; i++) {
        printf("%s \n", listPacket->user_names[i]);
    }
}

void successful_name_change(const char *name) {
    printf("Successfully changed name to %s. \n", name);
}

void name_already_exists(const char *name) {
    printf("%s already exists. Try another name. \n", name);
}
