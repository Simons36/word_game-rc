#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "server.h"

#define START_OP_CODE "SNG"
#define START_RESP_OP_CODE "RSG"

char * process_request(char * buffer_request);
int start_func(char * buffer_request);
int start_input_correct(char *input, int **resp);
char *parse_msg_start(int * arr_let_err);

#endif