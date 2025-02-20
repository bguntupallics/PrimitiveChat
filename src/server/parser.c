#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int server_parser(int argc, char *argv[]){
    int port;
    char c;

    while ((c = getopt (argc, argv, "p:")) != -1) {
        switch (c) {
            case 'p':
                printf("Port: %s\n", optarg);
                port = atoi(optarg);
                break;
            default:
                printf("Invalid Argument: %s\n", optarg);
                break;
        }
    }

    return port;
}
