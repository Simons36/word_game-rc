#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "server.h"

#define START_OP_CODE "SNG"
#define START_RESP_OP_CODE "RSG"

#define PLAY_OP_CODE "PLG"
#define PLAY_RESP_OP_CODE "RLG"

char * process_request(char * buffer_request);
int start_func(char * buffer_request);
int start_input_correct(char *input, int **resp);
char *parse_msg_start(int * arr_let_err);
int play_input_correct(char *input);
int plid_valid(char * plid);
char* play_func(char * input);
char * play_func_aux(int plid, char letter, int trial);
char valid_letter(char letter);

#endif