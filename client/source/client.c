#include "../include/client.h"
#include "../include/network.h"
#include "../include/commands.h"

int main(int argc, char *argv[]){
    char command[6];
    char plid[6];


    if(set_ips(argc, argv) != 0) return EXIT_FAILURE;

    while (1){
        if(scanf("%s", command) != 1) return EXIT_FAILURE;
        

        if(!strcmp(command, START_COM) || !strcmp(command, START_COM_SHORT)){
            if(scanf("%s", plid) != 1) return EXIT_FAILURE;
            start_command(plid);
        }else if(!strcmp(command, EXIT_COM)){
            break;
        }
    }
    return 0;
}

