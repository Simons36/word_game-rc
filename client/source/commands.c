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
            printf("Error connecting to the server: there is already a game with this plid\n");
            return EXIT_FAILURE;
        }

        if(!strcmp(temp, "OK")){
            int n_letters, max_errors;
            sscanf(&resp[7], "%d %d", &n_letters, &max_errors);
            create_game_data(n_letters, max_errors);
            printf("New game started (max %d errors):\n", max_errors);
            print_word();
        }
    }else if(!strcmp(temp, "ERR")){
        printf("Error: Invalid plid\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int play_command(char * plid, char *letter){
    char **plg_msg = calloc(4, sizeof(char*));
    void *msg = malloc(2);
    size_t msg_len;
    char trials_str[12];

    char resp[128];

    plg_msg[0] = (char*)malloc(strlen(PLAY_MSG) + 1);
    strcpy(plg_msg[0], PLAY_MSG);

    plg_msg[1] = (char*)malloc(strlen(plid) + 1);
    strcpy(plg_msg[1], plid);

    plg_msg[2] = (char*)malloc(sizeof(char) + 1);
    strcpy(plg_msg[2], letter);

    plg_msg[3] = (char*)malloc(sizeof(char) + 1);
    sprintf(trials_str, "%d", get_trials() + 1);
    strcpy(plg_msg[3], trials_str);

    msg_len = parse_msg(plg_msg, msg, 4);

    strcpy(resp, send_msg_udp(msg, msg_len));

    free(plg_msg[0]);
    free(plg_msg[1]);
    free(plg_msg[2]);
    free(plg_msg[3]);
    free(msg);



    char temp[4];
    if(sscanf(resp, "%s", temp) != 1) exit(1);

    if(!strcmp(temp, PLAY_MSG_RESP)){
        int trial = 5;

        if(sscanf(&resp[strlen(PLAY_MSG_RESP)], "%s", temp) != 1) exit(1);

        sscanf(&resp[strlen(PLAY_MSG_RESP) + strlen(temp) + 2], "%d", &trial); // checks trial number sent by server, +2 for whitespaces
        set_trials(trial);

        if(!strcmp(temp, "DUP")){
            printf("Error: this letter was already sent in a previous trial\n");
        }else if(!strcmp(temp, "OVR")){
            increment_errors();
            printf("Game over: the number of maximum errors (%d) has been reached\n", get_max_errors());
            clear_game();
            return 1; //to signal to client main function that game is over, to clear plid
        }else if(!strcmp(temp, "INV")){
            printf("Invalid play command: the trial number, %d, was not the number expected\n", get_trials() + 1);
        }else if(!strcmp(temp, "ERR")){
            printf("Error play command: invalid PLID, or there is no ongoing game for this PLID, or the syntax of the play command was incorrect\n");
        }else{

            if(!strcmp(temp, "WIN")){
                play_win(letter[0]);
                return 1;
            }else if(!strcmp(temp, "NOK")){
                play_wrong_letter(letter[0]);
            }
            
            if(!strcmp(temp, "OK")){
                play_place_letter(letter[0], &resp[strlen(PLAY_MSG_RESP) + strlen(temp) + 4]);
            }

        }
        print_word();
        return 0;
    }else{
        //TODO: implement when server response is wrong
        return -1;
    }
}

void guess_command(char* plid, char* word){
    /*
    char **plg_msg = calloc(4, sizeof(char*));
    void *msg = malloc(2);
    size_t msg_len;
    char trials_str[12];

    char resp[128];
    char temp[4];

    plg_msg[0] = (char*)malloc(strlen(PLAY_MSG) + 1);
    strcpy(plg_msg[0], PLAY_MSG);

    plg_msg[1] = (char*)malloc(strlen(plid) + 1);
    strcpy(plg_msg[1], plid);

    plg_msg[2] = (char*)malloc(strlen(word) + 1);
    strcpy(plg_msg[2], word);

    plg_msg[3] = (char*)malloc(sizeof(char) + 1);
    sprintf(trials_str, "%d", get_trials() + 1);
    strcpy(plg_msg[3], trials_str);

    msg_len = parse_msg(plg_msg, msg, 4);

    strcpy(resp, send_msg_udp(msg, msg_len));

    free(plg_msg[0]);
    free(plg_msg[1]);
    free(plg_msg[2]);
    free(plg_msg[3]);
    free(msg);
    */
}

void ignore_line(){
    for(char c = ' '; c != '\n';c = getchar());
}

int plid_exists(char *plid){
    if(!strcmp(plid, "")){
        ignore_line();
        return FALSE;
    }else{
        return TRUE;
    }
}

void play_place_letter(char letter, char* n_pos){
    int num_times = 0;

    if(sscanf(n_pos, "%d", &num_times) != 1) exit(1); //numbers to read

    int numbs_read = 1;

    for(int i = 0; i < num_times; i++){
        int pos = 0;
        if(sscanf(&n_pos[2 * numbs_read], "%d", &pos) != 1) exit(1);
        numbs_read++;
        set_letter_by_pos(letter, pos);
    }
}

void play_wrong_letter(char letter){
    increment_errors();
    printf("Letter %c doesn't belong to the word\n", letter);
}

void play_win(char letter){
    complete_word(letter);
    print_word();
    printf("Congratulations! You have guessed the word and won the game\n");
}