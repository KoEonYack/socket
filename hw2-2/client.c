#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 512

int main(int argc,char* argv[]){
	int sock;							// 클라이언트쪽 소켓 디스크립터

	struct sockaddr_in serv_adr;		//ip address와 port number 등의 자료를 저장할 구조체
	int serv_adr_sz;

	FILE* fp=NULL;						// 파일 내용 read를 위한 파일 포인터 및 파일 모드 flag, 파일네임
	char file_mode=0;
	char filename[BUF_SIZE] = {0};
	char mode=0;

	char buf[BUF_SIZE] = {0};			// 클라이언트쪽 버퍼

	int read_cnt;						// UDP는 메세지 바운더리 서비스이므로,
	int read_total = 0;					// loss가 없다는 상황하에 전송한쪽 입장에서
	int str_len;						// 바라보면, 매회 버퍼사이즈 만큼 전송하다
	int i;								// 해당 버퍼사이즈보다 작은 메시지 길이 만큼 보내졌다면,
										// 종료 조건이다.
	

	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// 서버쪽에 전달할 파일명 입력 및 파일모드 입력
	printf("Enter the file name you want to send\n");
	scanf("%s",filename);                               
	printf("\nfile name check: %s\n",filename);

	do{
		printf("(A)scii or (B)inary file ? ");
		scanf(" %c",&mode);
	}while(!strchr("ab",tolower(mode)));

	mode=tolower(mode);

	if(mode=='b'){
		fp = fopen(filename,"rb");
		if(fp==NULL){
			perror("File Open Error");
			exit(1);
		}
		file_mode='b';
	}
	else{
		fp = fopen(filename,"rt");
		if(fp==NULL){
			perror("File Open Error");
			exit(1);
		}
		file_mode='t';
	}

	printf("\nfile mode check: %c\n",file_mode);

	// 소켓 생성       
	if((sock=socket(PF_INET, SOCK_DGRAM, 0))<0){
		perror("socket() error");
		exit(1);
	}
	printf("\nclient sockect created\n");

	// 구조체 초기화
	memset(&serv_adr, 0, sizeof(serv_adr));		
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	serv_adr_sz=sizeof(serv_adr);

	// 파일이름 및 파일입출력 모드 전달
	sendto(sock, filename, strlen(filename), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	sendto(sock, &file_mode, 1, 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	
	while(1){
		read_cnt = fread((void*)buf,1,BUF_SIZE,fp);	// 버퍼사이즈 만큼 1개씩 읽어서 buf에 저장함
		read_total += read_cnt;                     // read_total는 file에서 읽은 1 byte 단위의 char 총 개수!
		
		// UDP는 메세지 바운더리 서비스이므로,loss가 없다는 상황하에 전송한쪽 입장에서
		// 바라보면, 매회 버퍼사이즈 만큼 전송한다. 그러므로, 해당 버퍼사이즈보다 
		// 작은 메시지 길이 만큼 보내졌다면, 종료 조건이다.
		if(read_cnt < BUF_SIZE){
			sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
			printf("\ntotal file size : %d, filetransfer complete!\n",read_total);
			break;
		}
		sendto(sock, buf, BUF_SIZE, 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		printf("\ntotal sended : %d bytes",read_total);

		//버퍼 초기화
		for(i=0 ; i<BUF_SIZE ; i++){	
				buf[i]=0;
		}
	}

	fclose(fp);
	close(sock);
	return 0;
}