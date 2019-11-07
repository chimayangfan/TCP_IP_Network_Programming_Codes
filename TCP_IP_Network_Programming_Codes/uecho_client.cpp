#include"syscheck.h"

#define BUF_SIZE	1024

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef __linux
	int sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;
	struct sockaddr_in serv_adr, from_adr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while (1) {

		fputs("Insert message(q to quit):", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_adr, sizeof(serv_adr));//一次性发送完毕（不保证完全接收）
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_adr, &adr_sz);
		message[str_len] = 0;

		printf("Message from server: %s", message);
	}
	close(sock);
	return 0;
#endif

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET sock;
	char message[BUF_SIZE];
	int strLen;

	SOCKADDR_IN servAdr;
	if (argc != 3) {
		printf("Usage: %s <IP><port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		error_handling("socket() error!");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(sock, message, strlen(message), 0);
		strLen = recv(sock, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		printf("Message from server: %s", message);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
#endif

}

