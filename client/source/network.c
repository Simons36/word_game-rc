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
            int temp = 0;
            sscanf(argv[i+1], "%d", &temp);

            if(temp > PORT_MIN && temp < PORT_MAX)
                gs_ip_port.gsport = argv[i+1];
            else{
                printf("Invalid port: %s\n", argv[i+1]);
                return EXIT_FAILURE;
            }
        }else{
            printf("Invalid argument: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    if(DEV_MODE){
        printf("con: gsip: %s\n", gs_ip_port.gsip);
        printf("con: gsport: %s\n", gs_ip_port.gsport);
    }

    return 0;
}

char* send_msg_udp(void *buffer_msg, size_t len_msg){
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    static char buffer[128];

    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if(fd==-1) exit(1); //error


    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket

    struct timeval time_wait_response;
    time_wait_response.tv_sec = TIME_WAIT_RESPONSE;
    time_wait_response.tv_usec = 0;


    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&(time_wait_response), sizeof(struct timeval)); /*waits for response 5 seconds*/


    errcode = getaddrinfo(gs_ip_port.gsip, gs_ip_port.gsport, &hints,&res);
    if(errcode!=0){
        printf("Invalid ip: could not connect to server;");
        exit(1); //error
    } 
    /*
    */

    for(int i = 0; i < 3; i++){/*if server doesn´t respond try to send message three times*/

        printf("debug %s\n", (char*)buffer_msg);
        n = sendto(fd, buffer_msg, len_msg, 0, res->ai_addr, res->ai_addrlen);
        if(n==-1) exit(1); //error

        if(DEV_MODE){
            printf("con: sent: %zd\n", n);   
        }


        addrlen = sizeof(addr);
        n = recvfrom(fd,buffer,BUFFER_SIZE,0,
            (struct sockaddr*)&addr,&addrlen);

        if(n != -1){
            break;
        }else if(i == 2){
            printf("Error: server doesn't respond or message lost\n");
            exit(1);
        }else{
            printf("Server didn't respond: retrying to send messsage\n");
        }

    }


    if(DEV_MODE){
        printf("con: received: %zd\n", n);
        printf("con: msg: %s", buffer);
    }
    
    freeaddrinfo(res);
    close(fd);

    return &buffer[0];
}

void send_msg_tcp(void *buffer_msg, size_t len_msg){
    int fd,errcode;
    ssize_t n;
    struct addrinfo hints,*res;
    char buffer[128];

    fd=socket(AF_INET,SOCK_STREAM,0); //TCP socket
    if (fd==-1) exit(1); //error

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_STREAM; //TCP socket

    errcode = getaddrinfo(gs_ip_port.gsip, gs_ip_port.gsport,&hints,&res);
    if(errcode!=0)/*error*/exit(1);

    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1)/*error*/exit(1);

    n=write(fd,"Hello!\n",7);
    if(n==-1)/*error*/exit(1);

    n=read(fd,buffer,128);
    if(n==-1)/*error*/exit(1);

    n = write(1,"echo: ",6); if(n == -1) exit(1);

    n = write(1,buffer,n); if(n == -1) exit(1);

    freeaddrinfo(res);
    close(fd);
}

