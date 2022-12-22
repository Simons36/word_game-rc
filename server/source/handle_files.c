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

void add_play_game_file(int plid, char letter){
    char path[50];
    sprintf(path, "server/source/GAMES/GAME_%d.txt", plid);

    FILE *ptr = fopen(path, "a");
    char str[5];
    sprintf(str, "T %c\n", letter);

    fwrite(str, 1, strlen(str), ptr);

    fclose(ptr);
}

void add_guess_game_file(int plid, char *word){
    char path[50];
    sprintf(path, "server/source/GAMES/GAME_%d.txt", plid);

    FILE *ptr = fopen(path, "a");

    char str[40];
    sprintf(str, "G %s\n", word);

    fwrite(str, 1, strlen(str), ptr);

    fclose(ptr);
}

void move_game_file(int plid, char code){
    struct stat st = {0};

    char path[70];
    sprintf(path, "server/source/GAMES/%d", plid);

    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
    }

    time_t rawtime;
    struct tm * timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char new_filename[50];
    sprintf(new_filename, "/%d%02d%02d_%02d%02d%02d_%c.txt", 
            timeinfo->tm_year + 1900, 
            timeinfo->tm_mon + 1, 
            timeinfo->tm_mday,
            timeinfo->tm_hour, 
            timeinfo->tm_min, 
            timeinfo->tm_sec, 
            code);
    
    strcat(path, new_filename);

    char old_path[50];
    sprintf(old_path, "server/source/GAMES/GAME_%d.txt", plid);

    rename(old_path, path);
}