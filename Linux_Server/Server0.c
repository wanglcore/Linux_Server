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
#define max 10
char pass[4] = { '1','2','3','4' };
int main(int argc,char *argv[]) {
	int listenfd;
	int connfd = 0;
	struct sockaddr_in servaddr, clientaddr;
	char ip_addr[16], buff[1024];
	socklen_t clientaddr_len;
	int flag = 1;
	if (argc < 2) {//参数不足
		return 1;
	}
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {//奖励套接字
		return 1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));//初始化套接字
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {
		printf("set socket option error:");
		return 1;
	}
	if (bind(listenfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {//绑定
		printf("bind socket error:");
		return 1;
	}
	if (listen(listenfd, max) == -1) {//
		printf("listen socket error:");
		return 1;
	}
	/////密码验证
	while (1) {
		printf("===========waiting for client's connect===========\n");
		memset(&clientaddr, 0, sizeof(clientaddr));
		if ((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddr_len)) == -1) {
			printf("accept socket error:");
			return 1;
		}
		else {
			char *psword = "password:";
			if (send(connfd, psword, strlen(psword), 0) == -1) {
				printf("sent message error:");
				return 1;
			}
			else printf("message sent.\n");
			char passw[4];
			memset(passw, 0, sizeof(passw));
			int bytes = recv(connfd, passw, sizeof(passw), 0);
			if (bytes == -1) {
				printf("error");
				return 1;
			}
			
			for (int i = 0; i < sizeof(passw); i++) {
				if (pass[i] != passw[i]) {
					close(connfd);
					continue;
				}
			}
			break;
		}
	}
	inet_ntop(AF_INET, &clientaddr.sin_addr, ip_addr, 16);
	printf("client IP:%s, port:%d, connected\n", ip_addr, ntohs(clientaddr.sin_port));
	while (1) {
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
	close(connfd);
	close(listenfd);
	return 0;
}