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
        char *response;

        addrlen=sizeof(addr);
        n=recvfrom(fd,buffer,128,0,
            (struct sockaddr*)&addr,&addrlen);
        if(n==-1)/*error*/exit(1);

        
        k = write(1,"received: ",10);
        if(k != 10) exit(1);

        k = write(1,buffer,n);
        if(k != n) exit(1);
        
        response = process_request(buffer);

        n=sendto(fd,response,strlen(response) + 1,0,
            (struct sockaddr*)&addr,addrlen);
        if(n==-1)/*error*/exit(1);
    }
    freeaddrinfo(res);
    close(fd);
}

void tcp_connection(){
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    char buffer[128];

    fd=socket(AF_INET,SOCK_STREAM,0); //TCP socket
    if (fd==-1) exit(1); //error

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_STREAM; //TCP socket
    hints.ai_flags=AI_PASSIVE;

    errcode=getaddrinfo(NULL,SERVER_PORT,&hints,&res);
    if((errcode)!=0)/*error*/exit(1);

    n=bind(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1) /*error*/ exit(1);
    
    if(listen(fd,5)==-1)/*error*/exit(1);
    while(1){
    addrlen=sizeof(addr);
    if((newfd=accept(fd,(struct sockaddr*)&addr,
    &addrlen))==-1
    /*error*/ exit(1);
    n=read(newfd,buffer,128);
    if(n==-1)/*error*/exit(1);
    write(1,"received: ",10);write(1,buffer,n);
    n=write(newfd,buffer,n);
    if(n==-1)/*error*/exit(1);
    close(newfd);
    }
    freeaddrinfo(res);
    close(fd);
}