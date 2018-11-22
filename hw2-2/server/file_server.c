/**
    * UDP [Server] Socket Programming 
    * Buffer Size : 100 bytes
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

#define BUFSIZE 1024

void error_handling(char *message);

int main(int argc, char **argv)
{ 
    int serv_sock; 
    char message[BUFSIZE]; 
    int str_len, num=0;
    struct sockaddr_in serv_addr; 
    struct sockaddr_in clnt_addr; 
    int clnt_addr_size;
    
    FILE *fp;
    char file_type = 'x';
    char file_name[255];
    
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
   
    while(1) {
        clnt_addr_size=sizeof(clnt_addr);
//	memset(file_name, 0, BUFSIZE);
	str_len = recvfrom(serv_sock, file_name, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	file_name[str_len] = '\0';
	printf("Received File Name : %s \n", file_name);
	recvfrom(serv_sock, &file_type, 1, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size); 
	printf("Received File Type : %c\n", file_type);
        switch(file_type) {
		case 'a' :
			fp = fopen(file_name, "wt");
			break;
		case 'b' :
			fp = fopen(file_name, "wb");
			break;
		default :
			fp = fopen(file_name, "wb");
			break;
	}
	printf("Open file to write\n");
	while(str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size) != 0) {
		memset(message, 0, BUFSIZE);
		//printf("start write\n");
		message[str_len] = '\0';
		if(!strncmp(message, "end of file", 11)) 
			break;
		else{
			fwrite((void*)message, 1, str_len, fp);
			printf("success write\n");
		}
	}
	printf("finish write\n");
	fclose(fp);
      }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

