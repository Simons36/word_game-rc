#include "../include/commands.h"
#include "../include/data.h"
#include "../include/network.h"
#include "../include/game_data.h"

int start_command(char* plid){
    char **sng_msg = calloc(2, sizeof(char*));
    void *msg = malloc(2);
    size_t msg_len;

    char resp[128];
    char temp[4];
    

    sng_msg[0] = (char*)malloc(strlen(START_MSG) + 1);
    sng_msg[1] = (char*) malloc(strlen(plid) + 1);
    strcpy(sng_msg[0], START_MSG);
    strcpy(sng_msg[1], plid);

    msg_len = parse_msg(sng_msg, msg, 2);
    
    strcpy(resp, send_msg_udp(msg, msg_len));

    if(sscanf(resp, "%s", temp) != 1) return EXIT_FAILURE;

    if(!strcmp(temp, START_MSG_RESP)){
        if(sscanf(&resp[4], "%s", temp) != 1) return EXIT_FAILURE;
        
        if(!strcmp(temp, "NOK")){
            printf("Error connecting to the server: player already has ongoing game\n");
            return EXIT_SUCCESS;
        }

        if(!strcmp(temp, "OK")){
            int n_letters, max_errors;
            sscanf(&resp[7], "%d %d", &n_letters, &max_errors);
            create_game_data(n_letters, max_errors);
            printf("New game started (max %d errors):", max_errors);
            print_word();
        }
    }

    free(sng_msg[0]);
    free(sng_msg[1]);
    free(sng_msg);

    return EXIT_SUCCESS;
}