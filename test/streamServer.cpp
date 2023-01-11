#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>


#define MYPORT 3490
#define BACKLOG 10

/*
	모든 클라이언트들에게 "HElloWorld 출력해줌 "
*/

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

	/* 
		어떤 포트에 연결되었는지 (일단 소켓 열면 연 소켓을 현재 system 포트에 연결시캬줘야 함)
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
		/*
			요청이 들어온 client에 대한 새로운 socket fd 만들어줌 -> 하나의 값으로 두 개의 socket fd 갖...?
			원래거(fd)-> 기다리던 포트에서 계속 listen()
			새거(fd) -> send() recv() 준비 되도록
			int accept(int sockfd, void *addr, int *addrlen);
			sockfd: listen() 하고 있는 소켓 기술자
			addr: 로컨 struct sockaddr_in 포인터 (접속에 관한 정보 -> 어느 호스트에서 어느 포트이용 접속하려 하는지)
			addrlen: 해당 정수에 struct sockaddr_in 크기 미리 지정되어야 

		*/
		if((new_fd = accept(sck_fd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size)) == -1){
			perror("accept");
			continue;
		}

		std::cout << "server: got connect from " << inet_ntoa(their_addr.sin_addr) << std::endl;

		if (!fork()){ /* 하지만 우리는 포크를 뜨지 않지 않을 예정,,, 멀티 플렉싱으로 하니까..*/
			/*
				스트림 소켓이나 연결된 데이터그램 소켓 위에서 정보 주고받을 때 
				int send(int sockfd, const void *msg, int len, int flags);
				sockfd: socket() 통해서 얻었거나 accept()으로 새로 구한 데이터 보낼 소켓 fd
				msg: 보낼 데이터 가리키는 포틴터
				len: 보낼 데이터 바이트 수
				flag: 는 그냥 0으로 해야함(...오ㅑ지...??)
			*/
			if(send(new_fd, "hello world!\n", 14, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		close(new_fd);

		while(waitpid(-1, NULL, WNOHANG) > 0);
	}
}

