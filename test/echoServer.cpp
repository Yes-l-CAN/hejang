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


#define MPORT 4242
#define MAXFD 1000
#define MAXBUF 10

void printPacket(unsigned char *packet, unsigned int packetSize);


int main() {
	int serverSock_fd = -1;
	/* 	소켓 만드는데 사용
		int socket(int domain, int type, int protocol);
		domain : 통신영역, type: 프로토콜 타입, protocol: 어떤 프로토콜...? */
	serverSock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSock_fd < 0){
		std::cerr << "can not create socket" << std::endl;
		return -1;
	}

	/* socket의 통신 대상 지정을 위해 address 사용 -> struct ~*/
	struct sockaddr_in	servAddr;
	servAddr.sin_family = AF_INET; 		// 주소 체계를 구분하기 위한 변수 addr_in의 경우 항상 AF_INET
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//host IP 주소  -> gethostbyname() 통새서 직접 설정 가능
	servAddr.sin_port = htons(MPORT);	//보트번호!

	/* 
		socket() 함수로 생성한 소켓에 ip주소, 포트 번호=> 소켓 주소정보에 해당하는 정보 할당 
		어떤 포트에 연결되었는지 (일단 소켓 열면 연 소켓을 현재 system 포트에 연결시캬줘야 함)
		int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
		sockfd: socket() 함수로 얻은 소켓 fd
		my_addr: IP 주소에 관한 정보(ip주소, 포트번호)를 담고있는 sockaddr 구조체 포인터
		addrlen: sockaddr 구조체 크기 
	*/
	int res = bind(serverSock_fd, (struct sockaddr *) &servAddr, sizeof(servAddr));
	if (res < 0){
		std::cerr << "bind error" << std::endl;
		return -1;
	}

	/*
		클라이언트의 연결이 가능할 수 있도록 만들어줌 (약간 대기열로 생각하면 될 듯..! 대기열이니까 FIFO)
		외부로부터의 반드시 처리해야하는 접속이 올 경우 >> listen() -> accept()
		int listen(int sockfd, int backlog);
		sockfd: 보통 소켓 fd
		backlog: 접속 대기 큐 최대 연결 가능 수 (외부 연결은 대기 큐에서 accept() 호출 때 까지 기다려야 함, 보통 20	제한)
		** listen() 전에 bind() 호출 (랜덤 지정 포트에서 접속할 순 없으니까...) **
	*/
	if(listen(serverSock_fd, MAXFD) < 0){
		std::cerr << "listen error" << std::endl;
		return -1;
	}

	std::cout << "listen" << MPORT << "port" << std::endl;

	fd_set	reads;
	fd_set	cpy_reads;
	FD_ZERO(&reads);
	FD_SET(serverSock_fd, &reads);
	int maxFd = serverSock_fd;

	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	int clientSock_fd = -1;
	struct sockaddr_in	clientAddr;

	const int buffSize = MAXBUF;
	char buffer[buffSize];

	while(1){
		timeout.tv_sec =  1;
		timeout.tv_usec = 0;

		cpy_reads = reads;

		int ret = select(maxFd + 1, &cpy_reads, NULL, NULL, &timeout);
		if(ret < 0){
			std::cerr << "select error" << std::endl;
			break;
		} else if( ret == 0){
			std::cout << "timeout?" << std::endl;
			continue;
		}

		if (FD_ISSET(serverSock_fd, &cpy_reads)){
			unsigned int size = sizeof(clientAddr);
			clientSock_fd = accept(serverSock_fd, (struct sockaddr *) &clientAddr, &size);
			
			if(clientSock_fd < 0){
				std::cerr << "invalid client socket" << std::endl;
				std::cerr << "ClientSocket : " << clientSock_fd << std::endl;
				continue;
			}

			std::cout << "accept new client! " << std::endl;
			std::cout <<  "ClientSocket : " << clientSock_fd << std::endl;

			while(1){
				ret  = recv(clientSock_fd, buffer, buffSize, 0);
				if(ret < 0) {
					std::cerr << "couldn't recv client socket error" << std::endl;
					break;
				} else if(ret == 0){
					std::cerr << "receive client socket closed " << std::endl;
					break;
				}
			
				std::cout << "receive message!!" << std::endl;
				std::cout << "Client Socket : " << clientSock_fd << std::endl;
				std::cout << "read: " << ret << std::endl;

			//	ret = send(clientSock_fd, buffer, ret, 0);
				if(ret < 0){
					std::cerr << "couldn't send socket error" << std::endl;
					break;
				} 
				std::cout << "send to client socket : " << clientSock_fd << std::endl;
				std::cout << "write: " << ret << std::endl;

				printPacket((unsigned char *)buffer, ret);

			} //while (false);

		//	close (clientSock_fd);
			std::cout << "close client socket : " << clientSock_fd << std::endl;
		}
	}

	if(serverSock_fd != -1) {
		close(serverSock_fd);
	}

	return (0);
}

void	printPacket(unsigned char *packet, unsigned int packetSize) {

	for (unsigned int i = 0; i < packetSize; i++) {
		if ((i % 0x10) == 0x00) {
			fprintf(stdout, "0x%04X: ", i);
			fprintf(stdout, "%02X ", packet[i]);
			continue;
		}

		fprintf(stdout, "%02X ", packet[i]);

		if (i == 0) {
			continue;
		}

		if ((i % 0x10) == 0x0F) {
			fprintf(stdout, "| ");
			for (unsigned int j = i - 0x0F; j < i; j++) {
				if (packet[j] >= 0x20 && packet[j] < 0x7F) {
					fprintf(stdout, "%c", packet[j]);
				} else {
					fprintf(stdout, ".");
				}

				if ((j % 0x08) == 0x07) {
					fprintf(stdout, " ");
				}
			}
			fprintf(stdout, "\n");
			continue;
		}

		if ((i % 0x08) == 0x07) {
			fprintf(stdout, "  ");
			continue;
		}
	}

	int remain = packetSize % 0x10;
	if (remain != 0) {
		unsigned int space = (0x10 - remain) * 3;
		if (remain < 8) {
			space += 2;
		}
		for (; space > 0; space--) {
			fprintf(stdout, " ");
		}

		fprintf(stdout, "| ");
		for (int i = remain; i > 0; i--) {
			int j = packetSize - i;
			if (packet[j] >= 0x20 && packet[j] < 0x7F) {
				fprintf(stdout, "%c", packet[j]);
			} else {
				fprintf(stdout, ".");
			}

			if ((j % 0x08) == 0x07) {
				fprintf(stdout, " ");
			}
		}

		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
}
