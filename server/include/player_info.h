#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "server.h"

#define MAX_PLAYERS 100

typedef struct client_info{
    int plid;
    char *word_to_guess;
    int guesses;
}*sessions;


int check_player(int plid);
char * put_player(int plid);
char *pick_word_from_file(int n_line);

#endif