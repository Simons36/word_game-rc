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

char* put_player(int plid){
    if(check_player(plid)){
        printf("Player already has an ongoing game in the server\n");
        return NULL;
    }

    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i] == NULL){
            sess_info[i] = malloc(sizeof(struct client_info));
            sess_info[i]->plid = plid;
            sess_info[i]->guesses = 0;
            sess_info[i]->word_to_guess = pick_word_from_file(i);
            return sess_info[i]->word_to_guess;
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