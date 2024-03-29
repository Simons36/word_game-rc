#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "server.h"
#include "operations.h"

#include <time.h>
#include <sys/stat.h>

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
    int right_guesses;
    int max_errors;
    int letters_guessed[26];
    int *letters_left;
    int errors;
    char **words_sent;
}*sessions;

typedef struct letter_guessed_pos{
    int num;
    int pos[30];
}*letter_n_pos;


int check_player(int plid);
int *put_player(int plid, char **word, char **path_image);
char *pick_word_from_file(int n_line, char **path_image);
int get_guesses_max(int len);
int get_n_words(FILE *ptr);
int play_letter(int plid, char letter, int trial, int** n_pos);
sessions get_player(int plid);
char *get_word(int plid);
int get_current_guesses(int plid);
int get_file_name(char file_name[], int k);
void create_file(int k);

/**
 * @brief Removes an active game from the array of games
 * 
 * @param plid player that will be removed
 * @return EXIT_SUCCESS if everything went right, EXIT_FAILURE if there were errors
 */
int remove_player(int plid);

/**
 * @brief This function checks if the word sent by the client in the guess command was already sent
 * in a previous trial
 * 
 * @return TRUE if already played, FALSE otherwise
 */
int check_word_already_guessed(int plid, char *word);

/**
 * @brief Increment number of guesses of player with plid
 */
void increment_guesses(int plid);

/**
 * @brief Increment number of errors of player with plid
 */
void increment_errors(int plid);

/**
 * @brief Function to check if number of errors is over the number of max errors
 *
 * @return TRUE if errors > max errors, FALSE otherwise
 */
int check_over_error_limit(int plid);

/**
 * @brief Adds word to the wrong word list
 */
void add_wrong_word(int plid, char *word);

/**
 * @brief Get the file image to the word assigned to plid
 */
msg_file get_file_image(FILE *game_file_ptr, char *op_code);

char *find_filename(char *path);

msg_file parse_msg_file(char *path, char *op_code);

FILE * check_player_tcp(int plid);

ssize_t get_filesize(char *path);

#endif