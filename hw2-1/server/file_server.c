/**
    * TCP [Server] Socket Programming 
    * Buffer Size : 100 bytes
    * 
    * @ Auth          : KoEonyack, GyeowoonJeong
    * @ Start Day     : 181105_2305
    * @ Last Modified : 181116_2305
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

#define BUFSIZE 2048

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

    FILE* fp = NULL;
    char file_type;
    char file_name[200];

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
    while(1) {
        clnt_addr_size=sizeof(clnt_addr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    
        if(clnt_sock == -1)    
	    error_handling("accept() error");

        while(1) {
            printf("now we start\n");
            recv(clnt_sock, &str_len, sizeof(int), 0);
            memset(file_name, 0, sizeof(file_name));
            recv(clnt_sock, file_name, str_len, 0);
	    printf("received file name : %s\n", file_name);
            recv(clnt_sock, &file_type, 1, 0);
	    printf("received file type : %c\n", file_type);

            switch(file_type) {
                case 'a' : 
                	fp = fopen(file_name, "wt");
                	break;
            
            	case 'b' :
                	fp = fopen(file_name, "wb");
                	break;
            }

            int fp_block_sz = 0;
	    while(1){
		printf("start to write in file\n");
		memset(message, 0, sizeof(message));
		str_len=recv(clnt_sock, message, BUFSIZE, 0);
		if(str_len==0)
			break;
		fwrite(message, sizeof(char), str_len, fp);
	    }

            fclose(fp);
	    break;
        }

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

