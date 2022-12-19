#include "../include/player_info.h"
#include "time.h"
#include <stdio.h>
#include <string.h>

sessions sess_info[MAX_PLAYERS];

int check_player(int plid){
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (sess_info[i] != NULL){
            if (sess_info[i]->plid == plid){
                return TRUE;
            }
        }
    }
    return FALSE;
}

int *put_player(int plid)
{
    static int ret[2];

    if (check_player(plid)){
        printf("Player already has an ongoing game in the server\n");
        return NULL;
    }

    for (int i = 0; i < MAX_PLAYERS; i++){
        if (sess_info[i] == NULL){
            int len_word;

            sess_info[i] = malloc(sizeof(struct client_info));
            sess_info[i]->plid = plid;
            sess_info[i]->guesses = 0;
            sess_info[i]->right_guesses = 0;
            sess_info[i]->errors = 0;
            sess_info[i]->word_to_guess = pick_word_from_file(i);

            len_word = (int)strlen(sess_info[i]->word_to_guess);
            ret[0] = len_word;

            sess_info[i]->max_errors = get_guesses_max(len_word);
            ret[1] = sess_info[i]->max_errors;

            // sess_info[i]->letters_guessed = (int *)malloc(sizeof(int) * 26); // 26 letters in the alphabet
            for (int k = 0; k < 26; k++){
                sess_info[i]->letters_guessed[k] = FALSE;
            }

            sess_info[i]->letters_left = (int *)calloc(strlen(sess_info[i]->word_to_guess), sizeof(int));
            for (int k = 0; k < strlen(sess_info[i]->word_to_guess); k++){
                sess_info[i]->letters_left[k] = NOT_GUESSED;
            }

            sess_info[i]->words_sent = calloc(1, sizeof(char*));

            if (verbose_flag){
                printf("PLID=%d: new game; word = \"%s\" (%d letters)\n", plid, sess_info[i]->word_to_guess, len_word);
            }

            return ret;
        }
    }

    printf("Session limit exceeded\n");
    return NULL;
}

int remove_player(int plid){
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (sess_info[i]->plid == plid){
            free(sess_info[i]->letters_left);
            free(sess_info[i]);
            for(int k = 0; sess_info[i]->words_sent[k] != NULL; k++){
                free(sess_info[i]->words_sent[k]);
            }
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

char *pick_word_from_file(int n_line){
    srand(time(NULL));
    FILE *ptr, *ptrcpy;
    char *word = (char *)malloc(sizeof(char) * 30);

    ptr = fopen("server/source/words/words.txt", "r");
    ptrcpy = fopen("server/source/words/words.txt", "r");

    if (ptr == NULL){
        printf("Word file could not be opened\n");
        return NULL;
    }

    int r = rand();

    r = r % get_n_words(ptrcpy);

    for (int i = 0; i < r; i++){
        if (fscanf(ptr, "%*[^\n]\n") != 0)
            return NULL; // skip line
    }

    if (fscanf(ptr, "%s", word) != 1){
        printf("Error reading the word\n");
        return NULL;
    }

    return word;
}

int get_guesses_max(int len){
    if (len <= 6){
        return MAX_ERRORS_6_CHAR;
    }
    else if (len < 10){
        return MAX_ERRORS_7_10_CHAR;
    }
    else{
        return MAX_ERRORS_MORE_10;
    }
}

int get_n_words(FILE *ptr){
    int lines = 0;
    char ch;

    while (!feof(ptr)){
        ch = fgetc(ptr);
        if (ch == '\n'){
            lines++;
        }
    }
    return ++lines;
}

int play_letter(int plid, char letter, int trial_numb, int **n_pos){
    int k = -1;
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (sess_info[i]->plid == plid){
            k = i;
            break;
        }
    }

    if (verbose_flag){
        printf("PLID=%d: play letter \"%c\"", plid, letter);
    }

    if (sess_info[k]->letters_guessed[letter - 97] == TRUE){
        if (verbose_flag){
            printf("; letter was already guessed in previous trial\n");
        }
        return RETURN_PLAY_DUP;
    }
    else if (sess_info[k]->guesses + 1 != trial_numb){
        if (verbose_flag){
            printf("; trial number sent by client (%d) was not the one expected (%d)\n", sess_info[k]->guesses + 1, trial_numb);
        }
        return RETURN_PLAY_INV;
    }

    sess_info[k]->letters_guessed[letter - 97] = TRUE;
    sess_info[k]->guesses++;

    int n = 0; // number of hits
    int is_all_guessed = TRUE;
    int correct_guess = FALSE;

    for (int i = 0; i < strlen(sess_info[k]->word_to_guess); i++){
        if (sess_info[k]->word_to_guess[i] == letter){
            n++;
            n_pos[n - 1] = (int *)malloc(sizeof(int));
            *n_pos[n - 1] = i;
            n_pos[n] = NULL;
            sess_info[k]->letters_left[i] = GUESSED;
            correct_guess = TRUE;
        }
        else if (sess_info[k]->letters_left[i] == NOT_GUESSED){
            is_all_guessed = FALSE;
        }
    }

    if (is_all_guessed){
        if (verbose_flag){
            printf(" - WIN (game ended)\n");
        }
        sess_info[k]->right_guesses++;
        create_file(k);
        return RETURN_PLAY_WIN;
    }
    else if (correct_guess){
        if (verbose_flag){
            printf(" - %d hits; word not guessed\n", n);
        }
        sess_info[k]->right_guesses++;
        return RETURN_PLAY_OK;
    }
    else{
        sess_info[k]->errors++;
        if (sess_info[k]->errors > sess_info[k]->max_errors){
            if (verbose_flag){
                printf("; game already over, max errors(%d) was reached\n", sess_info[k]->max_errors);
            }
            return RETURN_PLAY_OVR;
        }

        if (verbose_flag){
            printf(" - 0 hits\n");
        }
        return RETURN_PLAY_NOK;
    }
}

sessions get_player(int plid){
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (sess_info[i]->plid == plid){
            return sess_info[i];
        }
    }
    return NULL;
}

char *get_word(int plid){
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (sess_info[i]->plid == plid){
            return sess_info[i]->word_to_guess;
        }
    }
    return NULL;
}

int get_current_guesses(int plid)
{
    sessions sess = get_player(plid);
    return sess->guesses;
}



void create_file(int k){
    FILE *fp;
    char file_path[100];
    char file_name[50];
    char file_line[100];
    int score;
    score = get_file_name(file_name, k);
    sprintf(file_path, "server/source/SCORES/%s", file_name);
    printf("%s\n", file_path);
    sprintf(file_line,"%d %d %s %d %d", score, sess_info[k]->plid, sess_info[k]->word_to_guess, sess_info[k]->right_guesses, sess_info[k]->guesses);
    printf("%s\n", file_line);
    fp = fopen(file_path, "w+");
    if ( fp == NULL){  
        printf("Failed to create file\n");
        exit(EXIT_FAILURE);
    } 
    else{
         fputs(file_line,fp);
    }
    fclose(fp);
}



int get_file_name(char file_name[], int k){
    time_t t;
    struct tm tm;
    int score;
    int plid;
    int year, month, day, hour, min, sec;
    plid = sess_info[k]->plid;
    t = time(NULL);
    tm = *localtime(&t);
    score = ((double)sess_info[k]->right_guesses / (double)sess_info[k]->guesses) * 100;
    year = tm.tm_year + 1900;
    month = tm.tm_mon + 1;
    day = tm.tm_mday;
    hour = tm.tm_hour;
    min = tm.tm_min;
    sec = tm.tm_sec;
    sprintf(file_name,"%d_%d_%d%d%d_%d%d%d.txt", score, plid, day, month, year, hour, min, sec);
    return score;
}

int check_word_already_guessed(int plid, char *word){
    int n_sess = 0;
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            n_sess = i;
            break;
        }
    }

    if(sess_info[n_sess]->words_sent == NULL){
        return FALSE;
    }

    for(int k = 0; sess_info[n_sess]->words_sent[k] != NULL; k++){
        if(!strcmp(sess_info[n_sess]->words_sent[k], word)){
            return TRUE;
        }
    }
    
    return FALSE;
}

void increment_guesses(int plid){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            sess_info[i]->guesses++;
            return;
        }
    }
}

void increment_errors(int plid){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            sess_info[i]->errors++;
            return;
        }
    }
}

int check_over_error_limit(int plid){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            if(sess_info[i]->errors > sess_info[i]->max_errors){
                return TRUE;
            }else{
                return FALSE;
            }
        }
    }
    return -1;
}

void add_wrong_word(int plid, char *word){
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(sess_info[i]->plid == plid){
            for(int k = 0;; k++){
                if(sess_info[i]->words_sent[k] == NULL){
                    sess_info[i]->words_sent = realloc(sess_info[i]->words_sent, sizeof(char*) * (k + 1));
                    sess_info[i]->words_sent[k] = malloc(sizeof(char) * (strlen(word) + 1));
                    strcpy(sess_info[i]->words_sent[k], word);
                    return;
                }
            }
        }
    }
}