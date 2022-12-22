#ifndef HANDLE_FILES_H
#define HANDLE_FILES_H

#include "server.h"
#include <sys/stat.h>
#include <time.h>

int create_file_game(int plid, char *word, char *path_hint);

void add_play_game_file(int plid, char letter);

void add_guess_game_file(int plid, char *word);

void move_game_file(int plid, char letter);

#endif