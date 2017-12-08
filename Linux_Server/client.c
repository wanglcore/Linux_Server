#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/time.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8888
int main(int argc,char *argv[]) {
	int sockfd;
	struct sockaddr_in servaddr;
	char buff[1024];
	if (argc != 3) {
		printf("./client <ipaddress> ,<port>\n");
		return 1;
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1) {
		printf("error,socket could not initiation");
		return 1;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1],&servaddr.sin_addr) == -1) {
		printf("INET_PTON IS ERROR");
		return 1;
	}
	if (connect(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("connect error");
		return 1;
	}
	printf("successfully");
	while (1) {
		memset(buff, 0, sizeof(buff));
		scanf("%s", buff);
		if (send(sockfd, buff, strlen(buff), 0) == -1) {
			printf("send error");
			break;
		}
		else
			printf("message have send");
		memset(buff, 0, sizeof(buff));
		int clrecv = recv(sockfd, buff, sizeof(buff), 0);
		if (clrecv == -1) {
			printf("recv message error");
			break;
		}
		else if (clrecv == 0)
			break;
		else {
			printf("message:%s\n", buff);
		}
	}
	close(sockfd);
	return 0;
}