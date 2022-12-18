#include "../include/commands.h"
#include "../include/data.h"
#include "../include/network.h"
#include "../include/game_data.h"

int start_command(char* plid){
    char msg_send[64];
    char resp[128];
    char temp[4];


    sprintf(msg_send, "%s %s\n", START_MSG, plid);

    strcpy(resp, send_msg_udp(msg_send, strlen(msg_send) + 1));

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
    char trials_str[12];

    char resp[128];

    sprintf(trials_str, "%d", get_trials() + 1);

    //msg_len = parse_msg(plg_msg, msg, 4);

    char msg_send[64];
    sprintf(msg_send, "%s %s %s %s\n", PLAY_MSG, plid, letter, trials_str);

    strcpy(resp, send_msg_udp(msg_send, strlen(msg_send) + 1));

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
            printf("Game over: the number of maximum errors (%d) has been reached\n", get_max_errors());
            clear_game();
            return 1; //to signal to client main function that game is over, to clear plid
        }else if(!strcmp(temp, "INV")){
            printf("Invalid play command: the trial number, %d, was not the number expected\n", get_trials() + 1);
        }else if(!strcmp(temp, "ERR")){
            printf("Error play command: invalid PLID, or there is no ongoing game for this PLID, or the syntax of the play command was incorrect\n");
        }else{

            if(!strcmp(temp, "WIN")){
                complete_word(letter[0]);
                func_win();
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

int guess_command(char* plid, char* word){
    char trials_str[12];
    sprintf(trials_str, "%d", get_trials() + 1);

    char resp[128];

    char msg_send[64];

    sprintf(msg_send, "%s %s %s %s\n", GUESS_MSG, plid, word, trials_str);
    strcpy(resp, send_msg_udp(msg_send, strlen(msg_send) + 1));

    char command[4];
    char status[4];
    int new_trials;

    if(sscanf(resp, "%s %s %d\n", command, status, &new_trials) != 3){
        printf("Wrong protocl message received by the server, last command not executed\n");
        return -1;
    }

    if(!strcmp(command, GUESS_MSG_RESP)){
        if(!strcmp(status, "DUP")){
            printf("Error: this word has already been guessed\n");
        }else if(!strcmp(status, "INV")){
            printf("Error: trial number sent by the client (%d) was not the one expected by the server\n", get_trials() + 1);
        }else if(!strcmp(status, "ERR")){
            printf("Error: the syntax of the command was incorrect, the PLID is invalid, or there is no ongoing game\n");
        }else{

            if(!strcmp(status, "WIN")){
                complete_full_word(word);
                func_win();
                return 1;
            }else if(!strcmp(status, "OVR")){
                printf("Game over: the number of max errors (%d) was reached\n", get_max_errors());
                clear_game();
                return 1;
            }else if(!strcmp(status, "NOK")){
                increment_errors();
                printf("Wrong guess: '%s' is not the correct word\n", word);
            }else{
                printf("Wrong protocol message received by the server, last command not executed\n");
                return -1;
            }
        }
    }else{
        printf("Wrong protocol message received by the server, last command not executed\n");
        return -1;
    }

    set_trials(new_trials);
    print_word();
    return 0;
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

void func_win(){
    print_word();
    printf("Congratulations! You have guessed the word and won the game\n");
    clear_game();
}


int quit_command(char* plid){
    char resp[64];
    char msg_send[64];
    sprintf(msg_send, "%s %s\n", QUIT_COM, plid);

    strcpy(resp, send_msg_udp(msg_send, strlen(msg_send) + 1));
    
    char temp[4];
    if(sscanf(resp, "%s", temp) != 1) exit(1);

    if(!strcmp(temp, QUIT_MSG_RESP)){
        if(sscanf(&resp[4], "%s", temp) != 1) exit(1);
        
        if(!strcmp(temp, "NOK")){
            printf("Error connecting to the server: there is no ongoing game\n");
            return EXIT_FAILURE;
        }

        if(!strcmp(temp, "OK")){
            clear_game();
        }
    }else if(!strcmp(temp, "ERR")){
        printf("Error: Invalid plid or syntax \n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


