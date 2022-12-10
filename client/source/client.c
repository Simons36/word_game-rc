#include "../include/client.h"
#include "../include/network.h"
#include "../include/commands.h"

int main(int argc, char *argv[]){
    char command[6];
    char plid[6] = "";

    if(set_ips(argc, argv) != 0) return EXIT_FAILURE;

    while (1){
        if(scanf("%s", command) != 1) return EXIT_FAILURE;
        

        if(!strcmp(command, START_COM) || !strcmp(command, START_COM_SHORT)){

            if(!strcmp(plid, "")){
                if(scanf("%s", plid) != 1) return EXIT_FAILURE;
                if(start_command(plid) == EXIT_FAILURE) strcpy(plid, "");
            }else{
                printf("Client already has game started, with player id: %s\n", plid);
                ignore_line(); /*ignore rest of line*/
            }

        }else if((!strcmp(command, PLAY_COM) || !strcmp(command, PLAY_COM_SHORT)) && plid_exists(plid)){
            char letter[2];

            if(scanf("%s", letter) != 1) return EXIT_FAILURE;
            play_command(plid, letter);

        }else if(!strcmp(command, EXIT_COM)){
            break;
        }else{
            printf("Invalid command: %s\n", command);
            ignore_line();
        }
    }
    return 0;
}