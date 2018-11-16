/**
    * UDP [Server] Socket Programming
    * Buffer Size : 5 bytes
    *
    * @ Auth          : KoEonyack, GyeowoonJeong
    * @ Start Day     : 181104_2215
    * @ Last Modified : 181104_2215
    * @ Link          : https://github.com/KoEonYack/socket/ (PRIVATE GIT)
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 5

void error_handling(char *message);

int main(int argc, char **argv)
{
    int serv_sock;
    char message[BUFSIZE];
    int str_len, num=0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1){
        error_handling("ERROR : Can't open socke");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1) {
        error_handling("bind() error");
    }

    sleep(10);

    while(1) {
        clnt_addr_size=sizeof(clnt_addr);
        sleep(1);
        str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        message[str_len]='\0';
        if(!strcmp(message,"q\n")) break;
        printf("Message from client : %s\n", message);
        // printf("수신 번호 : %d \n", num++);
        // sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
      }
    //printf("통신종료");
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
