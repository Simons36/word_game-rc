#include "../include/operations.h"

size_t process_request(char * buffer_request, size_t len){
    char command[4];
    char space;

    strncpy(command, buffer_request, 3);

    if(!strcmp(command, START_OP_CODE)){
        if(buffer_request[3] != ' '){
            return -1;
        }
        start_func(&buffer_request[4], len - 4);
    }/*else if{

    }*/
    return 0;
}

size_t start_func(char * buffer_request, size_t len){
    printf("str: %s\n", buffer_request);
    return 0;
}
