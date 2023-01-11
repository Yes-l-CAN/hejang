
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>


#define MYPORT 3440
#define BACKLOG 10

int main(){
	int sck_fd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in	their_addr;
	int	sin_size;

	/* 	소켓 만드는데 사용
		int socket(int domain, int type, int protocol);
		domain : 통신영역, type: 프로토콜 타입, protocol: 어떤 프로토콜...? */
	if((sck_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;	/*host byte order*/
	my_addr.sin_port = htons(MYPORT); /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /*auto fill with my IP*/
	bzero(&(my_addr.sin_zero), 8); /*zero the reset of the struct*/

	/* 어떤 포트에 연결되었는지 (일단 소켓 열면 연 소켓을 현재 system 포트에 연결시캬줘야 함)
		int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
		sockfd: socket() 함수로 얻은 소켓 fd
		my_addr: IP 주소에 관한 정보(ip주소, 포트번호)를 담고있는 sockaddr 구조체 포인터
		addrlen: sockaddr 구조체 크기 
	*/
	if(bind(sck_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
		perror("accept");
		//continue;
	}

	/*
		외부로부터의 반드시 처리해야하는 접속이 올 경우 >> listen() -> accept()
		int listen(int sockfd, int backlog);
		sockfd: 보통 소켓 fd
		backlog: 접속 대기 큐 최대 연결 가능 수 (외부 연결은 대기 큐에서 accept() 호출 때 까지 기다려야 함, 보통 20	제한)
		** listen() 전에 bind() 호출 (랜덤 지정 포트에서 접속할 순 없으니까...) **
	*/
	if(listen(sck_fd, BACKLOG) == -1){
		perror("listen");
		exit(1);
	}

	while(1) { /* main accept() loop*/
		sin_size = sizeof(struct sockaddr_in);
		if((new_fd = accept(sck_fd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
			perror("accept");
			continue;
		}

		printf("server: got connect from %s\n", inet_ntoa(their_addr.sin_addr) );

		if(!fork())
	}
}

