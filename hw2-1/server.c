#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 2048

int main(int argc, char* argv[])
{
	//서버와 클라이언트의 소켓 디스크립터
	int server_sd, client_sd;
	
	//ip address와 port number 등의 자료를 저장할 구조체
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	FILE* fp=NULL;			// 파일 생성을 위한 파일 포인터
	char file_mode;			// 파일 모드 flag : 바이너리, 아스키
	char file_name[255]={0};

	char msg[BUFSIZE];		//Receiver쪽의 버퍼

	int str_len;
	
	char client_ip_addr[INET_ADDRSTRLEN];
	int client_sin_size;
	

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 서버쪽 소켓 생성 및 에러체크
	if((server_sd=socket(PF_INET,SOCK_STREAM,0))<0){
		perror("socket error");
		exit(1);
	}
	
	// ip address와 port number 할당을 위한 구조체 초기화
	memset(&serv_addr,0,sizeof(serv_addr));

	// Address Family = IPv4 , Ip Address 32비트 빅엔디안 형으로 저장, 포트넘버 int값으로 변환 후, 16비트 빅엔디안 형으로 저장
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	// 서버쪽 소켓 디스크립터와 ip address, portnumber를 바인드 및 에러체크!!
	if(bind(server_sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
		perror("bind error");
		exit(1);
	}

	// 연결 요청 대기상태를 위한 listen 함수 호출! 대기 큐의 크기 5로 설정! 서버소켓으로서의 역할 시작!
	if(listen(server_sd, 5)<0){
		perror("listen error");
		exit(1);
	}

	// Iterative server를 위한 반복문
	while(1){
		// 대기 큐에서, 첫 번째로 대기 중에 있는 연결요청을 참조하여, 클라이언트와의 연결 구성 및 에러체크!
		client_sin_size=sizeof(clnt_addr);
		if((client_sd=accept(server_sd,(struct sockaddr*)&clnt_addr,&client_sin_size))<0){
			perror("Accept error");
			exit(1);
		}

		inet_ntop(clnt_addr.sin_family,&(clnt_addr.sin_addr),client_ip_addr,INET_ADDRSTRLEN);
		printf("Client %s is connected!! \n",client_ip_addr);

		while(1)
		{
			// TCP의 경우, 바이트 스트림이기 때문에, 버퍼가 다 차지 않았다면,
			// 이름과 내용 정보를 같이 한꺼번에 전송할 수 있기 때문에, 
			// 파일 이름 길이를 받아와서, 바운더리를 체크해준다. 그 후, file_name에 구분하여 저장함!
			recv(client_sd,&str_len,sizeof(int),0);

			memset(file_name,0,sizeof(file_name));
			recv(client_sd,file_name, str_len, 0);

			recv(client_sd, &file_mode, 1, 0);

			// 입력 받은 모드 정보에 맞게 file open
			if(file_mode=='t'){
				fp = fopen(file_name,"wt");
			}
			else{
				fp = fopen(file_name, "wb");
			}

			while(1){
				memset(msg, 0, sizeof(msg));
				str_len=recv(client_sd, msg, BUFSIZE, 0);

				// 클라이언트로 부터, 강제 close 되었거나, 파일 전송이 끝났다면 break!!
				if(str_len==0)
					break;

				fwrite(msg, sizeof(char), str_len, fp);
			}
			
			// 파일 전송이 끝났다면, file close 시키고 break
			fclose(fp);
			break;

		}
		// client_sd를 close시키고 다시 iterative하게 동작!!
		close(client_sd);
		printf("Client %s is disconnectd\n",client_ip_addr);
	}

	close(server_sd);
	return 0;
}