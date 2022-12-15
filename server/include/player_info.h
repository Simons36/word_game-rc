#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "server.h"

#include <time.h>

#define MAX_PLAYERS 100
#define MAX_ERRORS_6_CHAR 7
#define MAX_ERRORS_7_10_CHAR 8
#define MAX_ERRORS_MORE_10 9
#define N_WORDS 3

#define RETURN_PLAY_DUP 1
#define RETURN_PLAY_OVR 2
#define RETURN_PLAY_INV 3
#define RETURN_PLAY_WIN 4
#define RETURN_PLAY_OK 5
#define RETURN_PLAY_NOK 6

#define GUESSED 1
#define NOT_GUESSED 0

typedef struct client_info{
    int plid;
    char *word_to_guess;
    int guesses;
    int max_errors;
    int letters_guessed[26];
    int *letters_left;
    int errors;
}*sessions;

typedef struct letter_guessed_pos{
    int num;
    int pos[30];
}*letter_n_pos;


int check_player(int plid);
int* put_player(int plid);
char *pick_word_from_file(int n_line);
int get_guesses_max(int len);
int get_n_words(FILE *ptr);
int play_letter(int plid, char letter, int trial, int** n_pos);
sessions get_player(int plid);
char *get_word(int plid);
int get_current_guesses(int plid);

#endif