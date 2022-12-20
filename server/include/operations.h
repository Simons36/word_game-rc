#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "server.h"

#define MSG_ERROR "ERR\n"

#define MSG_GUESS_ERROR "RWG ERR\n"

typedef struct msgfile{
    char *filename;
    ssize_t f_size;
    FILE *file;
}*msg_file;

char * process_request(char * buffer_request);
int start_func(char * buffer_request);
int start_input_correct(char *input, int **resp, int plid);
char *parse_msg_start(int * arr_let_err);
int play_input_correct(char *input);
int plid_valid(char * plid);
char* play_func(char * input);
char * play_func_aux(int plid, char letter, int trial);
char valid_letter(char letter);
char* parse_msg_play_ok(int **n_pos, int trial);
int get_len_n_pos(int ** n_pos);

char* quit_func(char *input);
char *guess_func(char* input);
/**
 * @brief Checks if a word in the guess command is valid
 */
int word_valid(char *word);

/**
 * @brief Auxilary function of guess_func
 * 
 * @return Message to be sent to the client
 */
char *guess_func_aux(int plid, char *word, int trials);

void scoreboard_func();

char* hint_func();

#endif