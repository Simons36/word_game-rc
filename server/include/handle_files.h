#ifndef HANDLE_FILES_H
#define HANDLE_FILES_H

#include "server.h"
#include <sys/stat.h>
#include <time.h>

int create_file_game(int plid, char *word, char *path_hint);

void add_play_game_file(int plid, char letter);

void add_guess_game_file(int plid, char *word);

void move_game_file(int plid, char letter);

int count_lines(FILE *ptr);

int check_letter_in_word(char *word, char letter);

char *create_temp_file(char *path, int plid, char type);

void erase_temp_files(char *op_code_status, char *filename);

int belongs_to_list(char *letters_guessed, char letter);

char *get_filename(char *path);

#endif