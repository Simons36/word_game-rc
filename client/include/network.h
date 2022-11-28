#ifndef NETWORK_H
#define NETWORK_H

#define LOCALHOST_IP "127.0.0.1"
#define BUFFER_SIZE 128

#include "client.h"
#include "../../common/network_common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct gs_net{
    char *gsip;
    char *gsport;
}gs_net_info;

extern gs_net_info gs_ip_port;

int set_ips(int argc, char * argv[]);
void send_msg_udp(void *buffer_msg, size_t len_msg);

#endif