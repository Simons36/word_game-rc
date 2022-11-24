#ifndef NETWORK_H
#define NETWORK_H

#define LOCALHOST_IP "127.0.0.1"
#define SERVER_PORT "58015"

#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void set_ips(int argc, char * argv[]);
void udp_send();

#endif