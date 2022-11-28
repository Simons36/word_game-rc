#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "server.h"

#define START_OP_CODE "SNG"

size_t process_request(char * buffer_request, size_t len);

size_t start_func(char * buffer_request, size_t len);

#endif