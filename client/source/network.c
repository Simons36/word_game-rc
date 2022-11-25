#include "../include/network.h"

void set_ips(int argc, char* argv[]){
    gsip = LOCALHOST_IP;
    gsport = (char*)calloc(6, sizeof(char));
    sprintf(gsport, "%d", 58000);
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-n")){
            gsip = argv[i+1];
        }else if(!strcmp(argv[i], "-p")){
            gsport = argv[i+1];
        }
    }
}