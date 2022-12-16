#include "../include/data.h"


size_t parse_msg(char** args_array, void *msg, int argc){
    size_t len_msg = 0;
    void *msg_count = msg; 

    for(int i = 0; i < argc; i++){
        msg = realloc(msg, strlen(args_array[i]) + len_msg + 1);
        memcpy(msg_count, (void*) args_array[i], strlen(args_array[i]));
        len_msg += strlen(args_array[i]);
        msg_count = msg + len_msg;
        if(i != argc - 1){
            memset(msg_count,' ', 1);
            msg_count += 1;
        }
        len_msg += 1;
    }
    memset(msg_count, '\n', 1);
    return len_msg;
}
/*
size_t parse_msg(char** args_array, void *msg, int argc){
    size_t len_msg = 0;
    void *msg_count = msg;

    for(int i = 0; i < argc; i++){
        len_msg += strlen(args_array[i]) + 1;
    }

    msg = realloc(msg, sizeof(char) * len_msg);

    for(int i = 0; i < argc; i++){
        memcpy(msg_count, args_array[i], strlen(args_array[i]));
        msg_count += strlen(args_array[i]);
        if(i != argc - 1){
            memset(msg_count, ' ', 1);
        }else{
            memset(msg_count, '\n', 1);
        }
        msg_count += 1;
    }

    return len_msg;

}
*/