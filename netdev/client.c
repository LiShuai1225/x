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
	unsigned int fromLen;
	char recvBuffer[128];

	if (argc < 3) {
		printf("usage: %s <server_ip> <string>\r\n", argv[0]);
		exit(0);
	}

	/* 创建socket */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		printf("create socket fail.\r\n");
		exit(1);
	}
	/* 设置目标地址 */
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;	/* 通信协议 */
	toAddr.sin_addr.s_addr = inet_addr(argv[1]);	/* 目标IP 地址，来自命令行 */
	toAddr.sin_port = htons(4000);	/* 目标端口 */

	printf("try to send: %s\n", argv[2]);
	/* 发送字符串到目标地址 */
	if (sendto
	    (sock, argv[2], strlen(argv[2]), 0, (struct sockaddr *)&toAddr,
	     sizeof(toAddr)) != strlen(argv[2])) {
		printf("send fail, errno=%d.\r\n", errno);
		close(sock);
		exit(1);
	}
	printf("send successful\n");

	fromLen = sizeof(fromAddr);
	/* 接收消息 */
	if (recvfrom
	    (sock, recvBuffer, 128, 0, (struct sockaddr *)&fromAddr,
	     &fromLen) < 0) {
		printf("receive fail, errno=%d\r\n", errno);
		close(sock);
		exit(1);
	}

	printf("recvive data: %s\r\n", recvBuffer);

	close(sock);

	return 0;
}
