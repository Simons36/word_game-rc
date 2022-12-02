#include "../include/player_info.h"

sessions sess_info[MAX_PLAYERS];

int check_player(int plid){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i] != NULL){
            if(sess_info[i]->plid == plid){
                return TRUE;
            }
        }
    }
    return FALSE;
}

int* put_player(int plid){
    static int ret[2];

    if(check_player(plid)){
        printf("Player already has an ongoing game in the server\n");
        return NULL;
    }

    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i] == NULL){
            int len_word;

            sess_info[i] = malloc(sizeof(struct client_info));
            sess_info[i]->plid = plid;
            sess_info[i]->guesses = 0;
            sess_info[i]->word_to_guess = pick_word_from_file(i);

            len_word = (int) strlen(sess_info[i]->word_to_guess);
            ret[0] = len_word;

            sess_info[i]->max_errors = get_guesses_max(len_word);
            ret[1] = sess_info[i]->max_errors;
            printf("debug: %d\n", ret[0]);
            return ret;
        }
    }

    printf("Session limit exceeded\n");
    return NULL;

}

char *pick_word_from_file(int n_line){
    FILE * ptr;
    char *word = (char*)malloc(sizeof(char) * 30);

    ptr = fopen("server/source/words/words.txt", "r");

    if(ptr == NULL){
        printf("Word file could not be opened\n");
        return NULL;
    }

    for(int i = 0; i < n_line; i++){
        if(fgets(NULL, 128, ptr) != NULL){// to ignore warning
            return NULL;
        }
    }

    if(fscanf(ptr, "%s", word) != 1){
        printf("Error reading the word\n");
        return NULL;
    }

    return word;
}

int get_guesses_max(int len){
    if(len <= 6){
        return MAX_ERRORS_6_CHAR;
    }else if(len < 10){
        return MAX_ERRORS_7_10_CHAR;
    }else{
        return MAX_ERRORS_MORE_10;
    }
}