#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define CLIENT_BUF_SIZE 512

int main(int argc,char* argv[]){

	int serv_sock;						// 서버 소켓 디스크립터

	struct sockaddr_in serv_adr;		//ip address와 port number 등의 자료를 저장할 구조체
	struct sockaddr_in clnt_adr;
	int clnt_adr_sz;		

	FILE* fp;							// 파일 생성을 위한 파일 포인터 및 파일 모드 flag, 파일네임
	char file_mode=0;
	char filename[BUF_SIZE] = {0};
	
	int str_len=0;

	int write_cnt=0;

	char buf[BUF_SIZE] = {0};
	
	int i;
	

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 소켓 생성
	if((serv_sock=socket(PF_INET, SOCK_DGRAM, 0))<0){
		perror("socket() error");
		exit(1);
	}
	printf("\nserver sockect created\n");

	// 구조체 초기화
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	clnt_adr_sz=sizeof(clnt_adr);

	if((bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))<0){
		perror("bind() error");
		exit(1);
	}
	printf("\nbind succeed\n");

	// 파일을 주고 받음
	while(1){

		// 메세지 바운더리를 유지하므로, 클라이언트에서 입력한 파일네임의 크기 범위가 일치해야함!()
		if((str_len=recvfrom(serv_sock, filename, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz))>=8
			&&(str_len=recvfrom(serv_sock, filename, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz))<=12){
			printf("\nfilename: %s\n",filename);
		}
		else{
			filename[0]='t';
			filename[1]='e';
			filename[2]='s';
			filename[3]='t';
			filename[4]='.';
			filename[5]='j';
			filename[6]='p';
			filename[7]='g';
			filename[8]=0;
		}

		recvfrom(serv_sock, &file_mode, 1, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
		if(file_mode=='t'){
			fp= fopen(filename,"wt");
		}
		else
			fp=fopen(filename,"wb");


		while((str_len = recvfrom(serv_sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz))!= 0 ){
			fwrite((void*)buf,1,str_len,fp);
			write_cnt+=str_len;
			printf("\nwrite byte size : %d bytes",write_cnt);

			// UDP는 메세지 바운더리 서비스이므로,loss가 없다는 상황하에 수신자 입장에서
			// 바라보면, 매회 최대 자신의 버퍼사이즈 만큼 전송 받는다. 그러므로, 수신자 입장에서
			// 수신자쪽의 버퍼사이즈가 송신자쪽의 버퍼사이즈보다 크다면, 성공적으로 받았다면,
			// 송신자쪽의 버퍼사이즈 만큼 받았을 것이고, 만약 수신자쪽의 버퍼사이즈가 송신자쪽의
			// 버퍼사이즈보다 작다면, 넘어가는 부분에 대해서 로스가 발생할 것이다.
			
			// 따라서, 수신자쪽에서 위 모든 경우를 포괄하려면, 아래와 같아야 종료조건을 알 수 있게 된다.
			// 수신자측의 버퍼사이즈가 송신자측의 버퍼사이즈보다 클 때, 종료 조건 str_len<CLIENT_BUF_SIZE
			// 수신자측의 버퍼사이즈가 송신자측의 버퍼사이즈보다 작을 때, 종료 조건 str_len<BUF_SIZE
			if(buf[str_len] == 0 && str_len < CLIENT_BUF_SIZE && str_len<BUF_SIZE)
				break;

			for(i=0 ; i<BUF_SIZE ; i++){
				buf[i]=0;
			}
		}
		printf("\ntotal file size : %d bytes",write_cnt);
		printf("File transfer ended\n");
		fclose(fp);
	}

	close(serv_sock);
	return 0;
}