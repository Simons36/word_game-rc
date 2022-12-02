#include "../include/server.h"
#include "../include/network.h"

int main(int argc, char* argv[]){/*setsockopt*/
    if(set_args(argc, argv) != 0) return EXIT_FAILURE;

    if(fork() == 0){
        tcp_connection();
    }else{
        udp_connection();
    }

    return 0;
}