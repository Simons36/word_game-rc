#include "../include/network.h"
#include "../include/operations.h"

char *gsip;
char *gsport;

void udp_connection(void *buffer_msg, size_t msg_len){
    int fd,errcode;
    ssize_t n, k;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    char buffer[128];

    /*create the socket*/
    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) /*error*/exit(1);

    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; // IPv4
    hints.ai_socktype=SOCK_DGRAM; // UDP socket
    hints.ai_flags=AI_PASSIVE;

    errcode=getaddrinfo(NULL, SERVER_PORT,&hints,&res);
    if(errcode!=0) /*error*/ exit(1);

    n=bind(fd,res->ai_addr, res->ai_addrlen);
    if(n==-1) /*error*/ exit(1);

    while (1){
        size_t len_response;

        addrlen=sizeof(addr);
        n=recvfrom(fd,buffer,128,0,
            (struct sockaddr*)&addr,&addrlen);
        if(n==-1)/*error*/exit(1);

        
        k = write(1,"received: ",10);
        if(k != 10) exit(1);

        k = write(1,buffer,n);
        if(k != n) exit(1);
        
        len_response = process_request(buffer, n);

        n=sendto(fd,buffer,n,0,
            (struct sockaddr*)&addr,addrlen);
        if(n==-1)/*error*/exit(1);
    }
    freeaddrinfo(res);
    close(fd);
}