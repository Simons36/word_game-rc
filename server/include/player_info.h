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

typedef struct client_info{
    int plid;
    char *word_to_guess;
    int guesses;
    int max_errors;
    int *letters_guessed;
}*sessions;


int check_player(int plid);
int* put_player(int plid);
char *pick_word_from_file(int n_line);
int get_guesses_max(int len);
int get_n_words(FILE *ptr);
int play_letter(int plid, char letter, int trial);
sessions get_player(int plid);

#endif