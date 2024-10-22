#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int main(int argc, char* argv[])
{
	int sock;
	char message[BUFSIZE];
	char file_mode;
	char file_name[255];
	char mode=0;
	int str_len;
	int fs_block_size;
	struct sockaddr_in serv_addr;

	FILE *fp;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	//socket call
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	//connect call
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		perror("connect() error!");
	}

	printf("[Connection is ready!!]\n");
	
	//Input file name
	printf("Please Enter the file name to send [%s][%s]server.\n", argv[1], argv[2]);
	scanf("%s",file_name);
	str_len = strlen(file_name); 
	//printf("\n\n%d\n\n",str_len);

	//파일 모드 입력
	do{
		printf("(A)scii or (B)inary file ? ");
		scanf(" %c", &mode);
	}while(!strchr("ab", tolower(mode)));

	mode = tolower(mode);
	
	/* 해당 파일 있는 지 검사 및 ascii인지 binary인지 확인*/
	if(mode == 'b'){
		fp = fopen(file_name, "rb");
		if(fp==NULL)
			perror("File Open Error");
		file_mode='b';
	}
	else{
		fp = fopen(file_name, "rt");
		if(fp==NULL)
			perror("File Open Error");
		file_mode='t';
	}
	
	printf("\n[File name:%s|str len:%d]\n",file_name, str_len);
	
	// 서버쪽에서 파일 이름과 contents를 구분하기 위해 filename 길이를 보내준다.
	send(sock, &str_len, sizeof(int),0);

	//  입력 받은 파일 이름을 전송
	send(sock, file_name, str_len, 0);

	// 파일 형태 (text, binary)를 보내줌.
	send(sock, &file_mode, 1, 0);

	// 파일 전송
	printf("\n[Start File Transfer]\n");
	memset(message, 0, sizeof(message));
	while((fs_block_size = fread(message, sizeof(char), sizeof(message), fp)) > 0){
		str_len=send(sock, message, fs_block_size, 0);
		if(str_len < 0){
			fprintf(stderr, "failed to send file %s\n", file_name);
			break;
		}
		memset(message, 0, sizeof(message));
	}

	printf("[File Transfer ends]\n");

	fclose(fp);
	getchar();
	close(sock);

	return 0;
}