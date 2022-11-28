#include "../include/commands.h"
#include "../include/data.h"
#include "../include/network.h"

int start_command(char* plid){
    char **sng_msg = calloc(2, sizeof(char*));
    void *msg = malloc(2);
    size_t msg_len;
    

    sng_msg[0] = (char*)malloc(strlen(START_MSG) + 1);
    sng_msg[1] = (char*) malloc(strlen(plid) + 1);
    strcpy(sng_msg[0], START_MSG);
    strcpy(sng_msg[1], plid);

    msg_len = parse_msg(sng_msg, msg, 2);
    
    send_msg_udp(msg, msg_len);

    free(sng_msg[0]);
    free(sng_msg[1]);
    free(sng_msg);

    return 0;
}