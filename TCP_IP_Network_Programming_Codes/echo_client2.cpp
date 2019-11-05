#include"syscheck.h"

#define BUF_SIZE	1024
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef linux
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;

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
	else {
		printf("Connected....");
	}

	while (1) {
		printf("Input message(Q to quit):");
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		str_len = write(sock, message, strlen(message));
		recv_len = 0;
		while (recv_len < str_len) {
			recv_cnt = read(sock, &message[10], BUF_SIZE - 1);//返回成功读取的字节数
			if (recv_cnt == -1)
				error_handling("read() error!");
			recv_len += recv_cnt;
		}
		message[str_len] = 0;

		printf("Message from server : %s \n", message);
	}
	close(sock);
	return 0;
#endif

#ifdef _WIN32
	int sock;
	WSADATA wsaData;
	int write, read, close;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage:%s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected........");
	while (1)
	{
		fputs("Input message (Q to quit):", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		str_len = send(sock, message, strlen(message), 0);

		recv_len = 0;
		while (recv_len<str_len)
		{
			recv_cnt = recv(sock, &message[recv_len], BUF_SIZE - 1, 0);
			//recv_cnt=read;
			if (recv_cnt == -1)
				error_handling("read()error");
			recv_len += recv_cnt;
		}
		message[recv_len] = 0;
		printf("Message from server:%s", message);
	}
	closesocket(sock);
	return 0;
#endif

}

