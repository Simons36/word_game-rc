#ifndef COMMANDS_H
#define COMMANDS_H

#include "client.h"

#define START_MSG "SNG"
#define START_MSG_RESP "RSG"

#define PLAY_MSG "PLG"
#define PLAY_MSG_RESP "RLG"

int start_command(char* plid);
void play_command(char * plid, char *letter);
int plid_exists(char * plid);
void play_place_letter(char letter, char * n_pos);
void ignore_line();
void play_wrong_letter(char letter);

/**
 * @brief Function used when server responds that client won the game;
 * fills out remaining spots with letter, prints word
 * and prints to the player winning message
 */
void play_win(char letter);

#endif