#include "../include/network.h"

void set_ips(int argc, char* argv[]){
    /*
    char buffer[128];
    if(gethostname(buffer, 128) == -1)
        fprintf(stderr,"error: %s\n",strerror(errno));

    printf("ip: %s\n", buffer);
        */
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-n")){
            gsip = argv[i+1];
        }else if(!strcmp(argv[i], "-p")){
            gsport = argv[i+1];
        }
    }
}