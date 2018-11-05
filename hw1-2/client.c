/**
    * UDP [Client] Socket Programming 
    * 
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

#define BUFSIZE 30

void error_handling(char *message);

int main(int argc, char **argv)
{ 
    int sock; 
    char message[BUFSIZE]; 
    int str_len;
    int addr_size;
    int i;
    
    // Send 10 btye dummy message
    char dummyMSG[] =  "0123456789"; 
    char dummyMSG2[] = "ABCDEFGHIJ"; 
    char dummyMSG3[] = "KLMNOPQRST";
    
    struct sockaddr_in serv_addr; 
    struct sockaddr_in from_addr;
    
    if(argc!=3){ 
        printf("Usage : %s <IP> <port>\n", argv[0]); 
        exit(1); 
    }
    
    sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        error_handling("ERROR : Can't open socket");
    }
  
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family=AF_INET; serv_addr.sin_addr.s_addr=inet_addr(argv[1]); 
    serv_addr.sin_port=htons(atoi(argv[2]));
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Send 10 bytes dummy message
    // sendto(sock, dummyMSG, strlen(dummyMSG), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    // sendto(sock, dummyMSG2, strlen(dummyMSG2), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    // sendto(sock, dummyMSG3, strlen(dummyMSG3), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    write(sock, dummyMSG, strlen(dummyMSG) + 1);
    write(sock, dummyMSG2, strlen(dummyMSG2) + 1);
    write(sock, dummyMSG3, strlen(dummyMSG3) + 1);
    while(1) {   
        fputs("Please input message (q to quit) : ", stdout); 
        fgets(message, sizeof(message), stdin); 

        if(!strcmp(message,"q\n")) break; 
        write(sock, message, strlen(message));

        // str_len=read(sock, message, sizeof(message)-1); 
        // message[str_len]=0;
        // printf("서버로부터 전송된 메시지 : %s", message);
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