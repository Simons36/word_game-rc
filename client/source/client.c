#include "../include/client.h"
#include"../include/network.h"

int main(int argc, char *argv[]){

    set_ips(argc, argv);
    printf("%s\n", gsport);

    return 0;
}

