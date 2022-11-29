#include "../include/operations.h"
#include "../include/player_info.h"

char * process_request(char * buffer_request){
    char command[4];
    

    strncpy(command, buffer_request, 3);

    if(!strcmp(command, START_OP_CODE)){
        char *resp = (char*)malloc(sizeof(char)*128);
        if(!start_input_correct(buffer_request, &resp)){
            return "RSG NOK\n";
        }
        strcat(resp, "\n");
        return resp;
    }/*else if{

    }*/
    return NULL;
}

int start_func(char * buffer_request){
    int plid;

    if(buffer_request[strlen(buffer_request) - 1] != '\n'){
        return EXIT_FAILURE;
    }

    sscanf(buffer_request, "%d", &plid);
    
    if(plid < 100000 || plid > 999999){
        printf("Error: invalid plid\n");
        return EXIT_FAILURE;
    }

    return plid;
}

int start_input_correct(char *input, char **resp){
    int plid;
    if(input[3] != ' ' || (plid = start_func(&input[4])) == EXIT_FAILURE){
        return FALSE;
    }
    if((*resp = put_player(plid)) == NULL){
        return FALSE;
    }
    return TRUE;
}