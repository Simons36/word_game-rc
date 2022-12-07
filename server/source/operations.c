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
        if(play_input_correct(buffer_request)){
            return play_func(&buffer_request[strlen(PLAY_OP_CODE)]);
        }else{
            return "RLG ERR\n";
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
    size_t count = strlen(PLAY_OP_CODE);

    if(input[count] != ' ')  return FALSE;
    
    count += 1;
    sscanf(&input[count], "%s", temp);

    if((plid = plid_valid(temp)) == FALSE || !check_player(plid)) return FALSE;
    
    count += strlen(temp);
    if(input[count] != ' ') return FALSE;

    count += 1;
    if(!(letter = valid_letter(input[count]))) return FALSE;

    
    count += 1;
    if(input[count] != ' ') return FALSE;

    count += 1;
    char trial = input[count];

    if(trial < '0' || trial > '9') return FALSE;

    count += 1;
    if(input[count] != '\n') return FALSE;

    return TRUE;

}

char* play_func(char * input){
    int plid;
    char letter, trial;
    char resp[128];
    
    sscanf(input, "%d %c %c", &plid, &letter, &trial);

    
 
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