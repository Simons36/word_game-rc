#include "../inc/client.h"

int main(int argc, char *argv[]){
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-n")){
            gsip = argv[i+1];
        }else if(!strcmp(argv[i], "-p")){
            gsport = argv[i+1];
        }
    }
    printf("%s\n", gsport);
    return 0;
}