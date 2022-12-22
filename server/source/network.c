#include "../include/network.h"
#include "../include/operations.h"

char *gsport;
int verbose_flag = FALSE;


int set_args(int argc, char *argv[]){
    gsport = SERVER_PORT;

    strcpy(word_file_global, argv[1]);

    for(int i = 2; i < argc; i+=2){
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

        /*
        ssize_t k;
        k = write(1,"received: ",10);
        if(k != 10) exit(1);

        k = write(1,buffer,n);
        if(k != n) exit(1);
        */

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
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
        printf("setsockopt(SO_REUSEADDR) failed");
        return exit(1);
    }
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

        msg_file resp = process_request_tcp(buffer);

        if(resp->file == NULL){
            n = write(newfd, resp->op_code_resp, strlen(resp->op_code_resp));
            if(n == -1) exit(1);
        }else{
            void *msg_begin = malloc(50 + resp->f_size);
            sprintf(msg_begin, "%s %s %zd ", resp->op_code_resp, resp->filename, resp->f_size);
            int size_begin = strlen(msg_begin);
            n = fread(msg_begin + size_begin, 1, resp->f_size, resp->file);
            ssize_t count_bytes = 0;

            while (count_bytes < resp->f_size){
                n = write(newfd, msg_begin + count_bytes, resp->f_size + size_begin - count_bytes);
                if(n == -1) exit(1);
                count_bytes += n;
            }
            free(msg_begin);

            erase_temp_files(resp->op_code_resp, resp->filename);
            
        }

        if(n==-1)/*error*/exit(1);
        close(newfd);
    }
    freeaddrinfo(res);
    close(fd);
}
