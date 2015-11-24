#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in toAddr;
	struct sockaddr_in fromAddr;
	int recvLen;
	unsigned int addrLen;
	char recvBuffer[128];

	/* 创建socket */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		printf("create socket fail!\r\n");
		exit(0);
	}

	/* 设置监听地址 */
	memset(&fromAddr, 0, sizeof(fromAddr));
	fromAddr.sin_family = AF_INET;
	fromAddr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地IP */
	fromAddr.sin_port = htons(4000);

	/* 绑定地址 */
	if (bind(sock, (struct sockaddr *)&fromAddr, sizeof(fromAddr)) < 0) {
		printf("bind address fail, errno=%d\r\n", errno);
		close(sock);
		exit(1);
	}

	while (1) {
		addrLen = sizeof(toAddr);
		/* 接收消息， toAddr保存的是消息来源地址 */
		if ((recvLen =
		     recvfrom(sock, recvBuffer, 128, 0,
			      (struct sockaddr *)&toAddr, &addrLen)) < 0) {
			printf("receive fail, errno=%d\r\n", errno);
			close(sock);
			exit(1);
		}

		printf("Server Rcv: %s\r\n", recvBuffer);
		/* 将消息回发到toAddr */
		if (sendto
		    (sock, recvBuffer, recvLen, 0, (struct sockaddr *)&toAddr,
		     sizeof(toAddr)) != recvLen) {
			printf("sendto fail\r\n");
			close(sock);
			exit(0);
		}
	}

	return 0;
}
