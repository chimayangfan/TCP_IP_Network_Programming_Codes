//迭代回声服务器端 / 客户端：
//服务器端在同一时刻只与一个客户端相连， 并提供回声服务。
//服务器端依次向5个客户端提供服务并退出。
//客户端接收用户输人的字符串并发送到服务器端。
//服务器端将接收的字符串数据传回客户端，即“回声”。
//服务器端与客户端之间的字符串回声一直执行到客户端输人Q为止。

#include"syscheck.h"

#define BUF_SIZE	1024
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef linux
	int serv_sock;
	int clnt_sock;

	char message[BUF_SIZE];
	int str_len, i;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error");
	}

	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	for (i = 0; i<5; i++) {
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) {
			error_handling("accept() error");
		}
		else {
			printf("Connected client %d\n", i + 1);
		}
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
		{
			write(clnt_sock, message, str_len);
			printf("client %d:message %s", i + 1, message);
}
		close(clnt_sock);
	}
	close(serv_sock);

	return 0;
#endif

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strLen, i;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	if (argc != 2)
	{
		printf("Usage:%s<port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		error_handling("socket() error!");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		error_handling("bind() error!");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		error_handling("listin() error");

	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i<5; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		if (hClntSock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
			send(hClntSock, message, strLen, 0);

		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
#endif

}

