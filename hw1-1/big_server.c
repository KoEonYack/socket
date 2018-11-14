/**
    * TCP [Server] Socket Programming 
    * Buffer Size : 100 bytes
    * 
    * @ Auth          : KoEonyack, GyeowoonJeong
    * @ Start Day     : 181105_2305
    * @ Last Modified : 1181105_2305
    * @ Link          : https://github.com/KoEonYack/socket/ (PRIVATE GIT)
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024

void error_handling(char *message);

int main(int argc, char **argv)
{
    int serv_sock;
    int clnt_sock;
    char message[BUFSIZE];
    int str_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    
    sleep(3); // TODO : 5 sec sleep. 

    if(clnt_sock==-1)
        error_handling("accept() error");

    /* 데이터 수신 및 전송 */
    while( (str_len=read(clnt_sock,message, BUFSIZE)) != 0){
        message[str_len]=0;
        printf("클라이언트로 부터 전송된 메시지 : %s", message);
        // send(clnt_sock, message, str_len, 0);
        // send(1, message, str_len, 0);
    }

    close(clnt_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
