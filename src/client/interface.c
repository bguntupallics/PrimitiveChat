#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "global.h"
#include "interface.h"
#include "client_outputs.h"
#include "client_network.h"

uint8_t connected = FALSE;
struct client client;

void sigint_handler(int signum) {
    (void)signum; // avoid unused parameter warning

    printf("\nClosing program... \n");

    if (connected) {
        disconnect(&client);
    }
    close_program();
}

void sigpipe_handler(int signum) {
    (void)signum;

    disconnect(&client);
}

void parse_command(char *command) {
    if(strcmp(command, "exit") == 0) {
        if(connected){
            disconnect(&client);
        }

        close_program();
    } else if(strcmp(command, "help") == 0) {
        help(connected);
    } else if(strcmp(command, "connect") == 0) {
        connect_to_server(&client, &connected);
    } else if(strcmp(command, "list") == 0) {
        list_users(&client);
    } else if(strcmp(command, "nickname") == 0) {
        change_name(&client);
    } else if(strcmp(command, "message")) {
        send_message(&client, 0);
    } else if(strcmp(command, "leave") == 0) {
        if(connected){
            disconnect(&client);
        } else {
            cant_leave_when_not_connected();
        }
    } else {
        invalid_command();
    }

    printf("~ ");
}

void run(void) {
    char command[2048];
    fd_set readfds;
    struct timeval tv;
    int ret, maxfd;

    // Install the SIGINT handler (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error installing SIGINT handler");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = sigpipe_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to Primitive Chat! Type \"help\" for a list of commands ->\n");
    printf("~ ");

    for(;;) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        maxfd = STDIN_FILENO;

        if(connected) {
            FD_SET(client.socketfd, &readfds);
            if(client.socketfd > STDIN_FILENO){
                maxfd = client.socketfd;
            }
        }

        tv.tv_sec = 2;
        tv.tv_usec = 0;
        fflush(stdout);

        ret = select(maxfd + 1, &readfds, NULL, NULL, &tv);

        if(ret < 0) {
            perror("select command. \n");
        } else if(ret == 0) {
            continue;
        } else {
            if(FD_ISSET(STDIN_FILENO, &readfds)) {
                scanf("%s", command);
                parse_command(command);
                memset(&command, 0, strlen(command));
            }

            if(connected && FD_ISSET(client.socketfd, &readfds)) {
                receive_message(&client);
            }
        }
    }
}
