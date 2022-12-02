#ifndef NETWORK_H
#define NETWORK_H

#include "server.h"
#include "../../common/network_common.h"

int set_args(int argc, char*argv[]);

void udp_connection();
void tcp_connection();

#endif