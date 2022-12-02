#include "../include/game_data.h"

game_data game_info;

void create_game_data(int n_letters, int max_errors){
    game_info = malloc(sizeof(struct gamedata));
    game_info->letters_guessed = calloc(n_letters, sizeof(char));
    for(int i = 0; i < n_letters; i++){
        game_info->letters_guessed[i] = ' ';
    }
    set_game_data(n_letters, max_errors);
}

void set_game_data(int n_letters, int max_errors){
    game_info->n_letters = n_letters;
    game_info->max_errors = max_errors;
}

int get_max_errors(){
    return game_info->max_errors;
}

int get_n_letters(){
    return game_info->n_letters;
}

char get_letter_by_pos(int pos){
    return game_info->letters_guessed[pos];
}

void print_word(){
    for(int i = 0; i < get_n_letters(); i++){
        if(get_letter_by_pos(i) == ' '){
            printf(" _");
        }
    }
    printf("\n");
}