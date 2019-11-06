#include"syscheck.h"

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef __linux
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect() error\r\n");
	}
	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1)
	{
		error_handling("read() error");
}

	printf("Message from server : %s \n", message);

	close(sock);
	return 0;
#endif

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		error_handling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		error_handling("connect() error\r\n");
	}
	str_len = recv(hSocket, message, sizeof(message) - 1, 0);
	if (str_len == -1)
	{
		error_handling("read() error");
	}

	printf("Message from server : %s \n", message);

	closesocket(hSocket);
	WSACleanup();
	return 0;
#endif

}

