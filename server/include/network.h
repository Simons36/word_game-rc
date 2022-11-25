#ifndef NETWORK_H
#define NETWORK_H

#include "server.h"
#include "../../common/include/network_common.h"

void udp_connection(void *buffer_msg, size_t msg_len);

#endif