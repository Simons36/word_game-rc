#ifndef NETWORK_H
#define NETWORK_H

#define LOCALHOST_IP "127.0.0.1"
#define BUFFER_SIZE 128

#include "client.h"
#include "../../common/include/network_common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

extern char *gsip;
extern char *gsport;

void set_ips(int argc, char * argv[]);
void udp_connection(void *buffer_msg, size_t len_msg);

#endif