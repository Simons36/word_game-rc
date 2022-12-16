#include "../include/client.h"
#include "../include/network.h"
#include "../include/commands.h"

int main(int argc, char *argv[])
{
    char command[6];
    char plid[6] = "";

    if (set_ips(argc, argv) != 0)
        return EXIT_FAILURE;

    while (1)
    {
        if (scanf("%s", command) != 1)
            return EXIT_FAILURE;

        if (!strcmp(command, START_COM) || !strcmp(command, START_COM_SHORT)){

            if (!strcmp(plid, "")){
                if (scanf("%s", plid) != 1)
                    return EXIT_FAILURE;
                if (start_command(plid) == EXIT_FAILURE)
                    strcpy(plid, "");
            }
            else{
                printf("Game has already been started, with player id: %s\n", plid);
            }
            ignore_line(); /*ignore rest of line*/

        }else if((!strcmp(command, PLAY_COM) || !strcmp(command, PLAY_COM_SHORT))){
            if(plid_exists(plid)){
                char *letter = (char*)malloc(sizeof(char)*2);

                if(scanf("%s", letter) != 1) EXIT_FAILURE;
                if(play_command(plid, letter) == 1) strcpy(plid, "");
                free(letter);
            }else{
                printf("Error: there is no ongoing game\n");
            }
            ignore_line();
        }
        else if (!strcmp(command, QUIT_COM) || !strcmp(command, EXIT_COM))
        {
            if (plid_exists(plid))
            {
                if (quit_command(plid) == 0) strcpy(plid, "");
            }
            else if (!strcmp(command, EXIT_COM)){
                return 0;
            }
            else
            {
                printf("Error: there is no ongoing game\n");
            }
            ignore_line();

        }else if(!strcmp(command, "guess")){
            if(plid_exists(plid)){
                char word[30] = "";

                if(scanf("%s", word) != 1) return EXIT_FAILURE;
                guess_command(plid, word);
            }else{

            }
        }
        else
        {
            printf("Invalid command: %s\n", command);
            ignore_line();
        }
    }
    return 0;
}