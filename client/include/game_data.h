#ifndef GAME_DATA_H
#define GAME_DATA_

#include "client.h"

typedef struct gamedata{
    int n_letters;
    int max_errors;
    char *letters_guessed;
    int n_trials;
    int n_errors;
}*game_data;

void create_game_data(int n_letters, int max_errors);
void set_game_data(int n_letters, int max_errors);
int get_n_letters();
int get_max_errors();
/**
 * @brief Get the letter by pos object
 * 
 * @param pos
 * @return char 
 */
char get_letter_by_pos(int pos);
void print_word();
void increment_trials();
int get_trials();

/**
 * @brief This function
 * sets in the game info struct (string letters_guessed)
 * the letter in the position pos
 */
void set_letter_by_pos(char letter, int pos);

/**
 * @brief Returns the number of errors in the current session
 */
int get_errors();

/**
 * @brief Increases the number of errors by one
 * 
 */
void increment_errors();

/**
 * @brief Complete all remaining spots with letter
 */
void complete_word(char letter);

/**
 * @brief Resets the game info stored in the client,
 * used when server communicates to the client 
 * that the game is over
 */
void clear_game();

/**
 * Sets the number of trials to trial
 * 
 */
void set_trials(int trial);

void complete_full_word(char *word);


#endif