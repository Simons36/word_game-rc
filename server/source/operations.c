#include "../include/operations.h"
#include "../include/player_info.h"
#include "../include/ist_operations.h"

char * process_request(char * buffer_request){
    char command[4] = "";
    

    strncpy(command, buffer_request, 3);

    //printf("%s\n", command);

    if(!strcmp(command, START_MSG)){
        int *r = (int*)malloc(sizeof(int)*2);
        int plid;
        int i_file;

        char *buffer_request2 = strdup(buffer_request);

        FindTopScores();
        create_scoreboard_file();

        if((plid = start_func(&buffer_request[strlen(START_MSG) + 1])) == EXIT_FAILURE){
            return MSG_ERROR;
        }

        if(!start_input_correct(buffer_request2, &r, plid)){
            return "RSG NOK\n";
        }else{
            return parse_msg_start(r);
        }

    }else if(!strcmp(command, PLAY_MSG)){

        return play_func(buffer_request);
        
    }else if(!strcmp(command, QUIT_MSG)){

        return quit_func(buffer_request);
        
    }else if(!strcmp(command, GUESS_MSG)){

        return guess_func(buffer_request);
    }else if(!strcmp(command, HINT_MSG)){
        return hint_func(buffer_request);
    }

    return MSG_ERROR;
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

int start_input_correct(char *input, int **r, int plid){
    if(input[strlen(START_MSG)] != ' '){
        return FALSE;
    }else if((*r = put_player(plid)) == NULL){
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

char* play_func(char *input){
    int plid;
    char temp[128];
    char letter;
    size_t count = strlen(PLAY_MSG);

    if(input[count] != ' ')  return "RLG_ERR\n";
    
    count += 1;
    sscanf(&input[count], "%s", temp);

    if((plid = plid_valid(temp)) == FALSE || !check_player(plid)) return "RLG ERR\n";
    
    count += strlen(temp);
    if(input[count] != ' ') return "RLG ERR\n";

    count += 1;
    if(!(letter = valid_letter(input[count]))) return "RLG ERR\n";

    count += 1;
    if(input[count] != ' ') return "RLG ERR\n";

    count += 1;
    int trial = 0;
    if(sscanf(&input[count], "%d", &trial) != 1) return "RLG ERR\n";

    if(trial < 0) return "RLG ERR\n";

    (trial < 10) ? (count += 1) : (count += 2);
    if(input[count] != '\n') return "RLG ERR\n";

    count += 1;
    if(input[count] != '\0') return "RLG ERR\n";


    return play_func_aux(plid, letter, trial);

}

char* play_func_aux(int plid, char letter, int trial){
    int **n_pos = calloc(1, sizeof(int*));
    int return_num = play_letter(plid, letter, trial, n_pos);

    static char str_return[30];
    strcpy(str_return, "RLG ");


    if(return_num == RETURN_PLAY_DUP){
        strcpy(&str_return[strlen(str_return)], "DUP ");
    }else if(return_num == RETURN_PLAY_OVR){
        strcpy(&str_return[strlen(str_return)], "OVR ");
    }else if(return_num == RETURN_PLAY_INV){
        strcpy(&str_return[strlen(str_return)], "INV ");
    }else if(return_num == RETURN_PLAY_WIN){
        strcpy(&str_return[strlen(str_return)], "WIN ");
    }else if(return_num == RETURN_PLAY_NOK){
        strcpy(&str_return[strlen(str_return)], "NOK ");
    }else{
        return parse_msg_play_ok(n_pos, trial);
    }

    char trial_str[13];
    sprintf(trial_str, "%d\n", get_current_guesses(plid));

    strcpy(&str_return[strlen(str_return)], trial_str);
    
    if(return_num == RETURN_PLAY_WIN || return_num == RETURN_PLAY_OVR){
        if(remove_player(plid) == EXIT_FAILURE){
            return NULL;
        }
    }
    return str_return;
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
        return letter + 32;
    }

    return FALSE;
}

char* parse_msg_play_ok(int **n_pos, int trial){
    static char resp[128];
    strcpy(resp, "RLG OK ");

    sprintf(&resp[strlen(resp)], "%d ", trial);

    int len = get_len_n_pos(n_pos);
    sprintf(&resp[strlen(resp)], "%d ", len);
    for(int i = 0; i < len; i++){
        sprintf(&resp[strlen(resp)], "%d ", *n_pos[i]);
    }
    strcpy(&resp[strlen(resp)], "\n");
    return resp;
}

int get_len_n_pos(int ** n_pos){
    int len = 0;
    for(int i = 0; n_pos[i] != NULL; i++){
        len++;
    }
    return len;
}



char* quit_func(char *input){

    int plid;
    char temp[128];
    size_t count = strlen(QUIT_MSG);

    if(input[count] != ' ')  return "RQT ERR\n";
    
    count += 1;
    sscanf(&input[count], "%s", temp);

    if((plid = plid_valid(temp)) == FALSE)return "RQT ERR\n";

    if(!check_player(plid)) return "RQT NOK\n";

    /*printf("%s", input);*/

    if(remove_player(plid) == EXIT_FAILURE){
        return "RQT ERR\n";
    }

    return "RQT OK\n";
}

char* guess_func(char *input){
    char op_code[4];
    char plid_str[20];
    char word[31];
    int trials, plid;
    if(sscanf(input, "%s %s %s %d", 
                                    op_code, 
                                    plid_str, 
                                    word, 
                                    &trials) != 4) return MSG_GUESS_ERROR;
    
    if((plid = plid_valid(plid_str)) == FALSE) return MSG_GUESS_ERROR;

    if(check_player(plid) == FALSE) return MSG_GUESS_ERROR;

    if(word_valid(word) == FALSE) return MSG_GUESS_ERROR;

    return guess_func_aux(plid, word, trials);
}

int word_valid(char *word){
    for(int i = 0; i < strlen(word); i++){
        if((word[i] < 'a' || word[i] > 'z') && (word[i] < 'A' || word[i] > 'Z')) return FALSE;
    }
    return TRUE;
}

char *guess_func_aux(int plid, char *word, int trials){
    static char resp[16];
    strcpy(resp, GUESS_MSG_RESP);
    int to_remove = FALSE;

    if(check_word_already_guessed(plid, word)){
        strcpy(&resp[strlen(GUESS_MSG_RESP)], " DUP ");

    }else if(trials != get_current_guesses(plid) + 1){
        strcpy(&resp[strlen(GUESS_MSG_RESP)], " INV ");

    }else{
        increment_guesses(plid);

        if(!strcmp(word, get_word(plid))){
            strcpy(&resp[strlen(GUESS_MSG_RESP)], " WIN ");
            to_remove = TRUE;
        }else{
            increment_errors(plid);
            if(check_over_error_limit(plid)){
                strcpy(&resp[strlen(GUESS_MSG_RESP)], " OVR ");
                to_remove = TRUE;
            }else{
                add_wrong_word(plid, word);
                strcpy(&resp[strlen(GUESS_MSG_RESP)], " NOK ");
            }
        }
    }

    int trials_to_send = get_current_guesses(plid);
    sprintf(&resp[strlen(resp)], "%d\n", trials_to_send);

    if(to_remove){
        remove_player(plid);
    }

    return resp;
}

char *hint_func(char *buffer_request){
    char op_code[4];
    char plid_str[20];
    int plid;

    if(sscanf(buffer_request, "%s %s\n", op_code, plid_str) != 2) return "RHL NOK\n";

    if((plid = plid_valid(plid_str)) == FALSE) return "RHL NOK\n";

    if(check_player(plid) == FALSE) return "RHL NOK\n";

    msg_file msg_to_send = get_file_image(plid);

    
}