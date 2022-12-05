#ifndef COMMANDS_H
#define COMMANDS_H

#include "client.h"

#define START_MSG "SNG"
#define START_MSG_RESP "RSG"

#define PLAY_MSG "PLG"

int start_command(char* plid);
void play_command(char * plid, char *letter);
int plid_exists(char * plid);

void ignore_line();

#endif