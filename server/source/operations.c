#include "../include/operations.h"
#include "../include/player_info.h"

char * process_request(char * buffer_request){
    char command[4];
    

    strncpy(command, buffer_request, 3);

    printf("%zd\n", strlen(buffer_request));

    if(!strcmp(command, START_OP_CODE)){
        int *r = (int*)malloc(sizeof(int)*2);

        if(!start_input_correct(buffer_request, &r)){
            return "RSG NOK\n";
        }
        return parse_msg_start(r);

    }else if(!strcmp(command, PLAY_OP_CODE)){
        if(!play_input_correct(buffer_request)){
            return NULL;
        }
    }//else if{

    //}
    return NULL;
}

int start_func(char * buffer_request){
    char plid[6];
    int plid_ret = 0;

    if(buffer_request[strlen(buffer_request) - 1] != '\n'){
        return EXIT_FAILURE;
    }

    sscanf(buffer_request, "%s", plid);

    if(strlen(buffer_request) - 1 != 6){
        printf("Error: '%s' invalid plid\n", buffer_request);
        return EXIT_FAILURE;
    }


    if((plid_ret = plid_valid(plid)) == FALSE) return EXIT_FAILURE; /*if player id is not valid
                                                                    then return failure; else
                                                                     convert plid to int in plid ret*/

    return plid_ret;
}

int start_input_correct(char *input, int **r){
    int plid;
    if(input[strlen(START_OP_CODE)] != ' ' || (plid = start_func(&input[strlen(START_OP_CODE) + 1])) == EXIT_FAILURE || (*r = put_player(plid)) == NULL){
        return FALSE;
    }
    return TRUE;
}


char* parse_msg_start(int * arr_let_err){
    static char resp[20];
    strcpy(resp, "RSG OK ");
    sprintf(&resp[strlen(resp)], "%d %d\n", arr_let_err[0], arr_let_err[1]);
    return resp;
}

int play_input_correct(char *input){
    int plid;
    char temp[128];
    char letter;
    size_t count;

    if(input[strlen(PLAY_OP_CODE)] != ' ')  return FALSE;
    
    sscanf(&input[strlen(PLAY_OP_CODE) + 1], "%s", temp);

    if((plid = plid_valid(temp)) == FALSE || !check_player(plid)) return FALSE;
    
    if(input[strlen(PLAY_OP_CODE) + 1 + strlen(temp)] != ' ') return FALSE;

    if(!(letter = valid_letter(input[strlen(PLAY_OP_CODE) + 1 + strlen(temp) + 1]))) return FALSE;

    if(input[strlen(PLAY_OP_CODE) + 1 + strlen(temp) + 2] != ' ') return FALSE;

    char trial = input[strlen(PLAY_OP_CODE) + 1 + strlen(temp) + 3];

    if(trial <= '0' || trial >= '9') return FALSE;

    if(input[strlen(PLAY_OP_CODE) + 1 + strlen(temp) + 4] != '\n') return FALSE;

    return TRUE;

}

int play_func(char * input){
    return 0;
}

int plid_valid(char * plid){
    int plid_ret = 0;

    if(strlen(plid) != 6){
        return FALSE;
    }
    
    for(int i = 0; i < strlen(plid); i++){
        if(plid[i] < '0' || plid[i] > '9'){
            printf("Error: invalid plid\n");
            return FALSE;
        }else{
            plid_ret = plid_ret*10 + (plid[i] - '0');
        }
    }
    return plid_ret;
}

char valid_letter(char letter){
    if(letter >= 'a' && letter <= 'z'){
        return letter;
    }

    if(letter >= 'A' && letter <= 'Z'){
        return letter - 32;
    }

    return FALSE;
}