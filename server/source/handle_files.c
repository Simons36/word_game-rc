#include "../include/handle_files.h"

int create_file_game(int plid, char *word, char *path_hint){
    char path_file_game[40];
    sprintf(path_file_game, "server/source/GAMES/GAME_%d.txt", plid);
    FILE *ptr = fopen(path_file_game, "w");

    if(ptr == NULL){
        return EXIT_FAILURE;
    }

    fprintf(ptr, "%s %s\n", word, path_hint);

    fclose(ptr);

    return EXIT_SUCCESS;
}