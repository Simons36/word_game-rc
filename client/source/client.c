#include "../include/client.h"
#include"../include/network.h"

int main(int argc, char *argv[]){

    set_ips(argc, argv);

    char *str = "Hello\n";

    void *buffer_msg = malloc(strlen(str));
    memcpy(buffer_msg, (void*) str, strlen(str));


    udp_connection(buffer_msg, strlen(str));
    return 0;
}

