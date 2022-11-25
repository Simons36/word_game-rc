#ifndef NETWORK_H
#define NETWORK_H

#define LOCALHOST_IP "127.0.0.1"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client.h"

struct addrinfo hints,*res,*p;
int errcode;
char buffer[INET_ADDRSTRLEN];
struct in_addr *addr;

void set_ips(int argc, char * argv[]);

#endif