#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<error.h>
#include<signal.h>
#include<wait.h>
#include<time.h>
#define PORT 8888
char psword[4] = { '1','2','3','4' };
int main(int argc,char *argv[]) {
	if (argc < 2) {
		return 1;
	}
	int flag = 1;
	struct sockaddr_in servaddr,clientaddr;
	socklen_t clientaddr_len;
	int listendfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listendfd == -1)
		return 1;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	if (setsockopt(listendfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {
		//
		return 1;
	}
	if (bind(listendfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		//
		return 1;
	}
	if (listen(listendfd, 5) == -1) {
		//
		return 1;
	}
	int fconn = 1;
	while (1) {
		//connect...
		memset(&clientaddr, 0, sizeof(clientaddr));
		int connfd = accept(listendfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
		if (connfd == -1) {
			//
			return 1;
		}
		char *password = "password:";
		send(connfd, password, strlen(password), 0);
		char passw[4];
		memset(passw, 0, sizeof(passw));
		int temp = recv(connfd, passw, sizeof(passw), 0);
		if (temp == -1) {
			//
			return 1;
		}
		for (int i = 0; i < sizeof(psword); i++) {
			if (psword[i] != passw[i]) {
				fconn = 0;
				char *fauword = "password_error";
				send(connfd, fauword, strlen(fauword),0);
				close(connfd);
				break;
			}
		}
		if (fconn) {
			break;
		}
	}
	char ip_addr[16];
	inet_ntop(AF_INET, &clientaddr.sin_addr, ip_addr, 16);
	printf("client IP:%s, port:%d,connseted\n", ip_addr, ntohs(clientaddr.sin_port));
	int connfd = 0;
	char buff[1024];
	while (1) {
		//´¦Àí
		printf("===========waiting for client's message===========\n");
		memset(buff, 0, sizeof(buff));
		int recvbytes = recv(connfd, buff, sizeof(buff), 0);
		if (recvbytes == -1) {
			printf("recv message error: ");
			break;
		}
		else if (recvbytes == 0) break;
		else printf("recv message: %s\n", buff);
		printf("===========type message sent to client===========\n");
		memset(buff, 0, sizeof(buff));
		scanf("%s", buff);
		if (send(connfd, buff, strlen(buff), 0) == -1) {
			printf("sent message error:");
			break;
		}
		else printf("message sent.\n");
	}
}