#include "../include/network.h"
#include "../include/operations.h"

char *gsport;
int verbose_flag = FALSE;


int set_args(int argc, char *argv[]){
    gsport = SERVER_PORT;

    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-v")){
            verbose_flag = TRUE;
        }else if(!strcmp(argv[i], "-p")){
            gsport = argv[i+1];
        }else{
            printf("Invalid argument: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    return 0;
}


void udp_connection(){
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;

    /*create the socket*/
    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) /*error*/exit(1);

    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; // IPv4
    hints.ai_socktype=SOCK_DGRAM; // UDP socket
    hints.ai_flags=AI_PASSIVE;

    errcode=getaddrinfo(NULL, gsport,&hints,&res);
    if(errcode!=0) /*error*/ exit(1);

    n=bind(fd,res->ai_addr, res->ai_addrlen);
    if(n==-1) /*error*/ exit(1);

    while (1){
        char *buffer = (char*)malloc(sizeof(char)*60);   
        char response[60];
        strcpy(buffer, "");

        addrlen=sizeof(addr);
        n=recvfrom(fd,buffer,60,0,
            (struct sockaddr*)&addr,&addrlen);
        if(n==-1)/*error*/exit(1);

        ssize_t k;
        k = write(1,"received: ",10);
        if(k != 10) exit(1);

        k = write(1,buffer,n);
        if(k != n) exit(1);

        strcpy(response, process_request(buffer));
    
        n=sendto(fd,response,strlen(response) + 1,0,
            (struct sockaddr*)&addr,addrlen);

        if(n==-1)/*error*/exit(1);
        free(buffer);
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
    int newfd;

    fd=socket(AF_INET,SOCK_STREAM,0); //TCP socket
    if (fd==-1) exit(1); //error

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_STREAM; //TCP socket
    hints.ai_flags=AI_PASSIVE;

    errcode=getaddrinfo(NULL,gsport,&hints,&res);
    if((errcode)!=0)/*error*/exit(1);

    n=bind(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1) /*error*/ exit(1);
    
    if(listen(fd,5)==-1)/*error*/exit(1);

    while(1){
        addrlen=sizeof(addr);

        if((newfd = accept(fd,(struct sockaddr*)&addr, &addrlen))==-1)
        /*error*/ exit(1);

        n=read(newfd,buffer,128);
        if(n==-1)/*error*/exit(1);

        char *resp = process_request(buffer);

        while (n > 0){
            n=write(newfd,resp,999999999);
        }
        

        if(n==-1)/*error*/exit(1);
        close(newfd);
    }
    freeaddrinfo(res);
    close(fd);
}
