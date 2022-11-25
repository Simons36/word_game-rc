#include "../include/network.h"

char *gsip;
char *gsport;
int plid;

void set_ips(int argc, char* argv[]){
    /*
    char buffer[128];
    if(gethostname(buffer, 128) == -1)
        fprintf(stderr,"error: %s\n",strerror(errno));

    printf("ip: %s\n", buffer);
        */
    gsip = LOCALHOST_IP;
    gsport = SERVER_PORT;

    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-n")){
            gsip = argv[i+1];
        }else if(!strcmp(argv[i], "-p")){
            gsport = argv[i+1];
        }
    }
}

void udp_connection(void *buffer_msg, size_t len_msg){
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    char buffer[128];

    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) exit(1); //error


    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket


    gsip = "tejo.tecnico.ulisboa.pt";
    errcode = getaddrinfo(gsip, gsport, &hints,&res);
    if(errcode!=0) exit(1); //error

    n = sendto(fd, buffer_msg, len_msg, 0, res->ai_addr, res->ai_addrlen);
    if(n==-1) exit(1); //error
    printf("sent: %zd\n", n);


    addrlen = sizeof(addr);
    n = recvfrom(fd,buffer,BUFFER_SIZE,0,
        (struct sockaddr*)&addr,&addrlen);

    if(n==-1) exit(1); //error
    printf("received: %zd\n", n);

    freeaddrinfo(res);
    close(fd);
}