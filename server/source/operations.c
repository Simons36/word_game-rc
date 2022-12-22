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
        //int i_file;

        char *buffer_request2 = strdup(buffer_request);

        

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
    }

    return MSG_ERROR;
}


msg_file process_request_tcp(char *buffer_request){
    char command[4] = "";

    strncpy(command, buffer_request, 3);

    if(!strcmp(command, HINT_MSG)){
        return hint_func(buffer_request);
    }else if (!strcmp(command, SCOREBOARD_MSG)){
        return scoreboard_func();
    }else if(!strcmp(command, STATE_MSG)){
        return state_func(buffer_request);
    }

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

int start_input_correct(char *input, int **r, int plid){
    char *word = calloc(30, sizeof(char));
    char *path_image = calloc(50, sizeof(char*));

    if(input[strlen(START_MSG)] != ' '){
        return FALSE;
    }else if((*r = put_player(plid, &word, &path_image)) == NULL){
        return FALSE;
    }

    if(create_file_game(plid, word, path_image) == EXIT_FAILURE){
        printf("Error creating the game file\n");
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
        add_play_game_file(plid, letter);
        move_game_file(plid, 'F');
    }else if(return_num == RETURN_PLAY_INV){
        strcpy(&str_return[strlen(str_return)], "INV ");
    }else if(return_num == RETURN_PLAY_WIN){
        strcpy(&str_return[strlen(str_return)], "WIN ");
        add_play_game_file(plid, letter);
        move_game_file(plid, 'W');
    }else if(return_num == RETURN_PLAY_NOK){
        strcpy(&str_return[strlen(str_return)], "NOK ");
        add_play_game_file(plid, letter);
    }else{
        add_play_game_file(plid, letter);
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
        sprintf(&resp[strlen(resp)], "%d ", *n_pos[i] + 1);
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

        add_guess_game_file(plid, word);
        if(!strcmp(word, get_word(plid))){
            strcpy(&resp[strlen(GUESS_MSG_RESP)], " WIN ");
            to_remove = TRUE;
            move_game_file(plid, 'W');
        }else{
            increment_errors(plid);
            if(check_over_error_limit(plid)){
                strcpy(&resp[strlen(GUESS_MSG_RESP)], " OVR ");
                to_remove = TRUE;
                move_game_file(plid, 'F');
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

msg_file hint_func(char *buffer_request){
    char op_code[4];
    char plid_str[20];
    int plid;
    FILE *game_file_ptr;

    if(sscanf(buffer_request, "%s %s\n", op_code, plid_str) != 2) return msg_error_tcp(MSG_HINT_ERROR);

    if((plid = plid_valid(plid_str)) == FALSE) return msg_error_tcp(MSG_HINT_ERROR);

    if((game_file_ptr = check_player_tcp(plid)) == NULL) return msg_error_tcp(MSG_HINT_ERROR);

    return get_file_image(game_file_ptr, "RHL OK");
}

msg_file msg_error_tcp(char* msg){
    msg_file msg_to_send = (msg_file)malloc(sizeof(struct msgfile));

    msg_to_send->file = NULL;

    msg_to_send->op_code_resp = (char*)malloc(sizeof(char)*15);
    strcpy(msg_to_send->op_code_resp, msg);

    return msg_to_send;
}

msg_file scoreboard_func(){

    if(FindTopScores() == 0){
        return msg_error_tcp("RSB EMPTY\n");
    }
    
    char *path = create_scoreboard_file();
    return parse_msg_file(path, "RSB OK");
}

msg_file state_func(char *buffer_request){
    char op_code[4];
    char plid_str[20];
    int plid;
    FILE *game_file_ptr;

    if(sscanf(buffer_request, "%s %s\n", op_code, plid_str) != 2) return msg_error_tcp(MSG_STATE_ERROR);

    if((plid = plid_valid(plid_str)) == FALSE) return msg_error_tcp(MSG_STATE_ERROR);

    game_file_ptr = check_player_tcp(plid);

    if(game_file_ptr != NULL){
        char path[50];
        sprintf(path, "server/source/GAMES/GAME_%d.txt", plid);
        create_temp_file(path);
        return parse_msg_file(path, "RST ACT");
    }else{

    }
    return NULL;
}