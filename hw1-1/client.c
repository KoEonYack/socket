/**
    * TCP [Client] Socket Programming 
    * 
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
#include <arpa/inet.h>

#define BUFSIZE 1024

void error_handling(char *message);

int main(int argc, char **argv)
{
    int sock;
    char message[BUFSIZE];
    int str_len;
    struct sockaddr_in serv_addr;

    // Send 10 btye dummy message
    char dummyMSG[] =  "0123456789"; 
    char dummyMSG2[] = "ABCDEFGHIJ"; 
    char dummyMSG3[] = "KLMNOPQRST";

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM, 0);
    
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        error_handling("connect() error!");
    }

    write(sock, dummyMSG, strlen(dummyMSG + 1));
    write(sock, dummyMSG2, strlen(dummyMSG2 + 1));
    write(sock, dummyMSG3, strlen(dummyMSG3 + 1));

    while(1) {
        /* 메세지 입력, 전송 */
        fputs("전송할 메시지를 입력 하세요 (q to quit) : ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message,"q\n")) break;

        write(sock, message, strlen(message));

        /* 메세지 수신, 출력 */
        // str_len=read(sock, message, BUFSIZE-1);
        // message[str_len]=0;
        // printf("서버로부터 전송된 메시지 : %s \n", message);
    }

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}