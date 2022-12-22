#ifndef COMMANDS_H
#define COMMANDS_H

#include "client.h"



/**
 * @brief Function that handles the start command
 * @return EXIT FAILURE if there was an error, 0 otherwise 
 */
int start_command(char* plid);

/**
 * @brief Function that handles the play command
 * 
 * @param letter Letter that is played
 * @return 1 if game is over (game won or max errors reached), 0 otherwise
 */
int play_command(char * plid, char *letter);

/**
 * @brief Function that handles guess command
 */
int guess_command(char *plid, char *word);

int plid_exists(char * plid);
void play_place_letter(char letter, char * n_pos);
void ignore_line();
void play_wrong_letter(char letter);
int quit_command(char* plid);

/**
 * @brief Function used when server responds that client won the game;
 * fills out remaining spots with letter, prints word
 * and prints to the player winning message
 */
void func_win();

void scoreboard_command();

void hint_command(char *plid);

void process_request_tcp(int socket);

void state_command(char *plid);

#endif
