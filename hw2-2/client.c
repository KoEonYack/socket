/**
    * UDP [Client] Socket Programming 
    * Data Sending.
    * 
    * @ Auth          : KoEonyack, GyeowoonJeong
    * @ Start Day     : 181104_2215
    * @ Last Modified : 181104_2215
    * @ Link          : https://github.com/KoEonYack/socket/ (PRIVATE GIT)
**/
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define BUFSIZE 32
#define cipherKey 'S' 
#define sendrecvflag 0 

void error_handling(char *message);
void clearBuf(char* b);
char Cipher(char ch);
int recvFile(char* buf, int s);

int main(int argc, char **argv) 
{ 
	int sockfd, nBytes; 
	// struct sockaddr_in addr_con; 
    // int addrlen = sizeof(addr_con);
    char net_buf[BUFSIZE]; 
    FILE* fp;
    
    struct sockaddr_in serv_addr; 
    int addrlen = sizeof(serv_addr);
    struct sockaddr_in from_addr;

    if(argc!=3){ 
        printf("Usage : %s <IP> <port>\n", argv[0]); 
        exit(1); 
    }

	sockfd = socket(PF_INET, SOCK_DGRAM, 0); 
	if (sockfd < 0) {
		error_handling("ERROR : Can't open socket");
    }

    // 이거
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]); 
    serv_addr.sin_port=htons(atoi(argv[2]));
    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while (1) { 
		printf("\nPlease enter file name to receive:\n"); 
		scanf("%s", net_buf); 
		sendto(sockfd, net_buf, BUFSIZE, 
			sendrecvflag, (struct sockaddr*)&serv_addr, 
			addrlen); 

		printf("\n---------Data Received---------\n"); 

		while (1) { 
			// receive 
			clearBuf(net_buf); 
			nBytes = recvfrom(sockfd, net_buf, BUFSIZE, 
							sendrecvflag, (struct sockaddr*)&serv_addr, 
							&addrlen); 

			// process 
			if (recvFile(net_buf, BUFSIZE)) { 
				break; 
			} 
		} 
		printf("\n-------------------------------\n"); 
	} 
	return 0; 
} 

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// funtion to clear buffer 
void clearBuf(char* b) 
{ 
	int i; 
	for (i = 0; i < BUFSIZE; i++) 
		b[i] = '\0'; 
} 

// function for decryption 
char Cipher(char ch) 
{ 
	return ch ^ cipherKey; 
} 

// function to receive file 
int recvFile(char* buf, int s) 
{ 
	int i; 
	char ch; 
	for (i = 0; i < s; i++) { 
		ch = buf[i]; 
		ch = Cipher(ch); 
		if (ch == EOF) 
			return 1; 
		else
			printf("%c", ch); 
	} 
	return 0; 
} 