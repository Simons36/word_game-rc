#include "../include/network.h"

gs_net_info gs_ip_port;

/**
 * @brief Set the server ip and port to connect to
 * 
 */
int set_ips(int argc, char* argv[]){
    gs_ip_port.gsip = LOCALHOST_IP;
    gs_ip_port.gsport = SERVER_PORT;

    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-n")){
            gs_ip_port.gsip = argv[i+1];
        }else if(!strcmp(argv[i], "-p")){
            gs_ip_port.gsport = argv[i+1];
        }else{
            printf("Invalid argument: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }
    printf("gsip: %s\n", gs_ip_port.gsip);
    printf("gsport: %s\n", gs_ip_port.gsport);
    return 0;
}

void send_msg_udp(void *buffer_msg, size_t len_msg){
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


    errcode = getaddrinfo(gs_ip_port.gsip, gs_ip_port.gsport, &hints,&res);
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