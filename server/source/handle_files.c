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

void create_temp_file(char *path){
    FILE *ptr_for_lines = fopen(path, "r");
    FILE *game_file = fopen(path, "r");
    char buffer[50] = "";
    char word[31];

    int lines = count_lines(ptr_for_lines);
    fclose(ptr_for_lines);

    lines--;//to ignore first line

    printf("lines %d\n", lines);

    if(fgets(buffer, 50, game_file) == NULL) exit(1);
    sscanf(buffer, "%c", word);

    FILE *temp_file = fopen("server/source/temp", "w+");
    char line_to_put[50] = "";

    sprintf(line_to_put, "--- Transactions found: %d ---\n", lines);

    fputs(line_to_put, temp_file);

    for(int i = 0; i < lines; i++){
        if(fgets(buffer, 50, game_file) == NULL) return;
        char type;
        char str_exist[8];

        sscanf(buffer, "%c", &type);


        if(type == 'T'){
            char letter;
            sscanf(&buffer[2], "%c", &letter);

            if(check_letter_in_word(word, letter)) strcpy(str_exist, "TRUE");
            else strcpy(str_exist, "FALSE");

            sprintf(line_to_put, "Letter trial: %c - %s\n", letter, str_exist);

        }else{
            char word_guessed[31];
            sscanf(&buffer[2], "%s", word_guessed);

            sprintf(line_to_put, "Word guess: %s\n", word_guessed);
        }

        fputs(line_to_put, temp_file);
    }
    fclose(game_file);
    fclose(temp_file);
}

int count_lines(FILE *ptr){
    int lines = 0;
    char ch;
    while(!feof(ptr)){
        ch = fgetc(ptr);
        if(ch == '\n'){
            lines++;
        }
    }
    return lines;
}

int check_letter_in_word(char *word, char letter){
    for(int i = 0; i < strlen(word); i++){
        if(word[i] == letter){
            return TRUE;
        }
    }
    return FALSE;
}