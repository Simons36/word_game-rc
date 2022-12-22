#include "../include/server.h"
#include "../include/network.h"

int main(int argc, char* argv[]){
    if(set_args(argc, argv) != 0) return EXIT_FAILURE;
    int n = fork();

    if(n == -1) exit(1);
    else if(n == 0) tcp_connection();
    else udp_connection();


    return 0;
}