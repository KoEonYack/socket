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

#define IP_PROTOCOL 0 
// #define IP_ADDRESS "127.0.0.1" // localhost 
// #define PORT_NO 15050 
#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0 

void error_handling(char *message);
void clearBuf(char* b);
char Cipher(char ch);
int recvFile(char* buf, int s);

int main(int argc, char **argv) 
{ 
	int sockfd, nBytes; 
	struct sockaddr_in addr_con; 
	int addrlen = sizeof(addr_con); 
	char net_buf[NET_BUF_SIZE]; 
	FILE* fp; 

    if(argc!=3){ 
        printf("Usage : %s <IP> <port>\n", argv[0]); 
        exit(1); 
    }

	sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 
	if (sockfd < 0) {
		error_handling("ERROR : Can't open socket");
	}

	addr_con.sin_family=AF_INET;
	addr_con.sin_addr.s_addr=inet_addr(argv[1]);
	addr_con.sin_port=htons(atoi(argv[2]));

	while (1) { 
		printf("\nPlease enter file name to receive:(q to quit)\n"); 
		scanf("%s", net_buf);
		// fgets(net_buf, sizeof(net_buf), stdin);
		// printf("\n%s\n", net_buf);
		if(!strcmp(net_buf, "q\n")) break;

		sendto(sockfd, net_buf, NET_BUF_SIZE, 
			sendrecvflag, (struct sockaddr*)&addr_con, 
			addrlen); 

		printf("\n---------Data Received---------\n"); 

		while (1) { 
			// receive 
			clearBuf(net_buf); 
			nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE, 
							sendrecvflag, (struct sockaddr*)&addr_con, 
							&addrlen); 

			// process 
			if (recvFile(net_buf, NET_BUF_SIZE)) { 
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
	for (i = 0; i < NET_BUF_SIZE; i++) 
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
