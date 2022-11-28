#include "../include/operations.h"

size_t process_request(char * buffer_request, size_t len){
    char command[3];

    memcpy(command, buffer_request, 3);

    if(!strcmp(command, START_OP_CODE)){

        start_func(buffer_request + 4, len - 4);
    }/*else if{

    }*/
}

size_t start_func(char * buffer_request, size_t len);
