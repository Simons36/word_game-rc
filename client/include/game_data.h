#ifndef GAME_DATA_H
#define GAME_DATA_

#include "client.h"

typedef struct gamedata{
    int n_letters;
    int max_errors;
    char *letters_guessed;
    int n_trials;
}*game_data;

void create_game_data(int n_letters, int max_errors);
void set_game_data(int n_letters, int max_errors);
int get_n_letters();
int get_max_errors();
char get_letter_by_pos(int pos);
void print_word();
void increment_trials();
int get_trials();

#endif