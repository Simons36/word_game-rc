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

    free(sng_msg[0]);
    free(sng_msg[1]);
    free(sng_msg);
    free(msg);

    if(sscanf(resp, "%s", temp) != 1) exit(1);

    if(!strcmp(temp, START_MSG_RESP)){
        if(sscanf(&resp[4], "%s", temp) != 1) exit(1);
        
        if(!strcmp(temp, "NOK")){
            printf("Error connecting to the server: there is already a game with this plid or invalid plid\n");
            return EXIT_FAILURE;
        }

        if(!strcmp(temp, "OK")){
            int n_letters, max_errors;
            sscanf(&resp[7], "%d %d", &n_letters, &max_errors);
            create_game_data(n_letters, max_errors);
            printf("New game started (max %d errors):", max_errors);
            print_word();
        }
    }

    return EXIT_SUCCESS;
}

void play_command(char * plid, char *letter){
    char **plg_msg = calloc(4, sizeof(char*));
    void *msg = malloc(2);
    size_t msg_len;
    int n_trials = get_trials();
    char trials_str[2];

    char resp[128];

    plg_msg[0] = (char*)malloc(strlen(PLAY_MSG) + 1);
    strcpy(plg_msg[0], PLAY_MSG);

    plg_msg[1] = (char*)malloc(strlen(plid) + 1);
    strcpy(plg_msg[1], plid);

    plg_msg[2] = (char*)malloc(sizeof(char) + 1);
    strcpy(plg_msg[2], letter);

    plg_msg[3] = (char*)malloc(sizeof(char) + 1);
    trials_str[0] = n_trials + '0';
    trials_str[1] = '\0';
    strcpy(plg_msg[3], trials_str);

    msg_len = parse_msg(plg_msg, msg, 4);

    strcpy(resp, send_msg_udp(msg, msg_len));
}

void ignore_line(){
    for(char c = ' '; c != '\n';c = getchar());
}