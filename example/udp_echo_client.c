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
    int str_len, addr_size, i;
    
    char MSG1[] = "Good "; 
    char MSG2[] = "Evening "; 
    char MSG3[] = "Everybody!";
    
    struct sockaddr_in serv_addr; 
    struct sockaddr_in from_addr;
    
    if(argc!=3){ 
        printf("Usage : %s <IP> <port>\n", argv[0]); 
        exit(1); 
    }
    
    sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        error_handling("UDP 소켓 생성 오류");
    }
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}