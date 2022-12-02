#include "../include/operations.h"
#include "../include/player_info.h"

char * process_request(char * buffer_request){
    char command[4];
    

    strncpy(command, buffer_request, 3);

    if(!strcmp(command, START_OP_CODE)){
        int *r = (int*)malloc(sizeof(int)*2);

        if(!start_input_correct(buffer_request, &r)){
            return "RSG NOK\n";
        }
        return parse_msg_start(r);

    }/*else if{

    }*/
    return NULL;
}

int start_func(char * buffer_request){
    char plid[6];
    int plid_ret = 0;

    if(buffer_request[strlen(buffer_request) - 1] != '\n'){
        return EXIT_FAILURE;
    }

    sscanf(buffer_request, "%s", plid);
    
    /*
    if(plid < 100000 || plid > 999999){
        printf("Error: invalid plid\n");
        return EXIT_FAILURE;
    }
    */

    if(strlen(buffer_request) - 1 != 6){
        printf("Error: '%s' invalid plid\n", buffer_request);
        return EXIT_FAILURE;
    }


    for(int i = 0; i < strlen(plid); i++){
        if(plid[i] < '0' || plid[i] > '9'){
            printf("Error: invalid plid\n");
            return EXIT_FAILURE;
        }else{
            plid_ret = plid_ret*10 + (plid[i] - '0');
        }
    }

    return plid_ret;
}

int start_input_correct(char *input, int **r){
    int plid;
    if(input[3] != ' ' || (plid = start_func(&input[4])) == EXIT_FAILURE || (*r = put_player(plid)) == NULL){
        return FALSE;
    }
    return TRUE;
}


char* parse_msg_start(int * arr_let_err){
    static char resp[20];
    strcpy(resp, "RSG OK ");
    sprintf(&resp[strlen(resp)], "%d %d\n", arr_let_err[0], arr_let_err[1]);
    printf("%s\n", resp);
    return resp;
}