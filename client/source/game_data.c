#include "../include/game_data.h"

game_data game_info;

void create_game_data(int n_letters, int max_errors){
    game_info = malloc(sizeof(struct gamedata));
    game_info->letters_guessed = calloc(n_letters, sizeof(char));
    for(int i = 0; i < n_letters; i++){
        game_info->letters_guessed[i] = ' ';
    }
    set_game_data(n_letters, max_errors);
    game_info->n_trials = 0;
    game_info->n_errors = 0;
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
    printf("Word:");
    for(int i = 0; i < get_n_letters(); i++){
        char letter;
        if((letter = get_letter_by_pos(i)) == ' '){
            printf(" _");
        }else{
            printf(" %c", letter);
        }
    }
    
    printf("   Lives remaining: %d\n", get_max_errors() - get_errors() + 1); 
    /*a "life" is how may errors u can make 
    before the game ends (max errors - current errors)*/                                                    
}

int get_trials(){
    return game_info->n_trials;
}

void increment_trials(){
    game_info->n_trials++;
}

void set_letter_by_pos(char letter, int pos){
    game_info->letters_guessed[pos] = letter;
}

int get_errors(){
    return game_info->n_errors;
}

void increment_errors(){
    game_info->n_errors++;
}

void complete_word(char letter){
    for(int i = 0; i < strlen(game_info->letters_guessed); i++){
        if(game_info->letters_guessed[i] == ' '){
            game_info->letters_guessed[i] = letter;
        }
    }
}

void clear_game(){
    free(game_info->letters_guessed);
    free(game_info);
}

void set_trials(int trial){
    game_info->n_trials = trial;
}