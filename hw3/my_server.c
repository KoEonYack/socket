#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BYTES 1024
void respond(int client);

int main(int argc, char *args[]) {

  int sock;
	int client;

  struct sockaddr_in addr;
	struct sockaddr_in client_addr;

	socklen_t addr_len = sizeof(addr);
  socklen_t clnt_adr_sz;

	int bind_port = 50000;
	int on = 1;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket() error");
		return 1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on)) != 0) {
		perror("setsockopt() error");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bind_port);
	if (bind(sock, (struct sockaddr*)&addr, addr_len) != 0) {
		perror("bind() error");
		return 1;
	}

	if (listen(sock, 5) != 0) {
		perror("listen() error");
		return 1;
	}

	printf("listening... %d\n", bind_port);
	while (1) {
		client = accept(sock, (struct sockaddr*)&client_addr, &clnt_adr_sz);
		if (client < 0) {
			perror("accept() error");
			return 1;
		}

		respond(client);
	}

	close(sock);

  return 0;
}

void respond(int client){
  char mesg[9999], *reqline[3], data_to_send[BYTES], path[99999];
  int rcvd, fd, bytes_read;
  char *ROOT; // Directory of html file

  memset( (void*)mesg, (int)'\0', 9999);

  rcvd = recv(client, mesg, 9999, 0);

  if (rcvd < 0){
    perror("recv() error\n");
  }
  else if (rcvd==0){
    perror("Client disconnected unexpectdly.\n");
  }
  else{
    printf("%s", mesg);
    reqline[0] = strtok(mesg, " \t\n");

    if(strncmp(reqline[0], "GET\0", 4)==0) // GET 으로 요청이 날라오는 경우
    {
      reqline[1] = strtok(NULL, " \t");
      reqline[2] = strtok(NULL, " \t\n");
      if (strncmp(reqline[2], "HTTP/1.0", 8) !=0 && strncmp(reqline[2], "HTTP/1.1", 8) != 0)
      {
        write(client, "HTTP/1.0 400 Bad Request\n", 25);
      }
      else
      {
        if( strncmp(reqline[1], "/\0", 2) == 0){
          reqline[1] = "./index.html";
        }

        ROOT = "./index.html";
        strcpy(path, ROOT);
        //strcpy(&path[strlen(ROOT)], reqline[1]);
        printf("file: %s\n", path);

        if ( (fd=open(path, O_RDONLY)) != -1) // file found
        {
          send(client, "HTTP/1.0 200 OK\n\n", 17, 0);
          while ( (bytes_read = read(fd, data_to_send, BYTES)) > 0 ){
            write(client, data_to_send, bytes_read);
          }
        }
        else{
          write(client, "HTTP/1.0 404 Not Found\n", 23); // file not found
        }
      }
    }
  }

  shutdown (client, SHUT_RDWR);
  close(client);
  client = -1;
}
