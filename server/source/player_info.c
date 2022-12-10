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

            //sess_info[i]->letters_guessed = (int *)malloc(sizeof(int) * 26); // 26 letters in the alphabet
            for (int k = 0; k < 26; k++)
            {
                sess_info[i]->letters_guessed[k] = FALSE;
            }

            sess_info[i]->letters_left = (int*)calloc(strlen(sess_info[i]->word_to_guess), sizeof(int));
            for(int k = 0; k < strlen(sess_info[i]->word_to_guess); k++){
                sess_info[i]->letters_left[k] = NOT_GUESSED;
            }

            if(verbose_flag){
                printf("PLID=%d: new game; word = \"%s\" (%d letters)\n", plid, sess_info[i]->word_to_guess, len_word);
            }

            return ret;
        }
    }

    printf("Session limit exceeded\n");
    return NULL;

}

char *pick_word_from_file(int n_line){
    srand(time(NULL));
    FILE * ptr, *ptrcpy;
    char *word = (char*)malloc(sizeof(char) * 30);

    ptr = fopen("server/source/words/words.txt", "r");
    ptrcpy = fopen("server/source/words/words.txt", "r");

    if(ptr == NULL){
        printf("Word file could not be opened\n");
        return NULL;
    }

    int r = rand();
    
    r = r % get_n_words(ptrcpy);

    for(int i = 0; i < r; i++){
        if(fscanf(ptr, "%*[^\n]\n") != 0) return NULL; //skip line
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

int get_n_words(FILE *ptr){
    int lines = 0;
    char ch;

    while(!feof(ptr)){
        ch = fgetc(ptr);
        if(ch == '\n'){
            lines++;
        }
    }
    return ++lines;
}

int play_letter(int plid, char letter, int trial_numb, int** n_pos){
    int k = -1;
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            k = i;
            break;
        }
    }

    if(sess_info[k]->guesses >= sess_info[k]->max_errors){
        return RETURN_PLAY_OVR;
    }else if(sess_info[k]->letters_guessed[letter - 97] == TRUE){
        return RETURN_PLAY_DUP;
    }else if(sess_info[k]->guesses != trial_numb){
        return RETURN_PLAY_INV;
    }

    sess_info[k]->letters_guessed[letter - 97] = TRUE;
    sess_info[k]->guesses++;

    int n = 0;
    int is_all_guessed = TRUE;
    int correct_guess = FALSE;
    
    for(int i = 0; i < strlen(sess_info[k]->word_to_guess); i++){
        if(sess_info[k]->word_to_guess[i] == letter){
            n++;
            printf("%d\n", n);
            if(n != 1){
                n_pos = realloc(n_pos, n - 1);
            }
            n_pos[n - 1] = (int*)malloc(sizeof(int));
            *n_pos[n - 1] = i;
            sess_info[k]->letters_left[i] = GUESSED;
            correct_guess = TRUE;
        }else if(sess_info[k]->letters_left[i] == NOT_GUESSED){
            is_all_guessed = FALSE;
        }
    }
    if(n_pos[4] == NULL){
        printf("hhhhhh\n");
    }
    printf("hmm: %d\n", *n_pos[0]);


    if(is_all_guessed){
        return RETURN_PLAY_WIN;
    }else if(correct_guess){
        return RETURN_PLAY_OK;
    }else{
        return RETURN_PLAY_NOK;
    }

}

sessions get_player(int plid){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            return sess_info[i];
        }
    }
    return NULL;
}

char *get_word(int plid){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            return sess_info[i]->word_to_guess;
        }
    }
    return NULL;
}