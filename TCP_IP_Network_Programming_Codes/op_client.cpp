#include"syscheck.h"

#define BUF_SIZE	1024
#define RLT_SIZE	4
#define OPSZ		4

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef linux
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, i;
	struct sockaddr_in serv_addr;

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

	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;

	for (i = 0; i<opnd_cnt; i++) {
		printf("Operand %d: ", i + 1);
		scanf("%d", (int *)&opmsg[i*OPSZ + 1]);
	}
	fgetc(stdin);	//end char
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt*OPSZ + 1]);
	write(sock, opmsg, opnd_cnt*OPSZ + 2);
	read(sock, &result, RLT_SIZE);

	printf("Operation result:%d \n", result);
	close(sock);
	return 0;
#endif

#ifdef _WIN32
	WSADATA wsadata;
	SOCKET hsocket;
	char opmsg[BUF_SIZE];
	int opndCnt, result, i;
	SOCKADDR_IN servAdr;
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		error_handling("WSAStartup() error!");

	//创建socket
	hsocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hsocket == INVALID_SOCKET) 
		error_handling("socket() error!\n");

	memset(&servAdr, 0, sizeof(servAdr));//清空
	servAdr.sin_family = AF_INET;		//ipv4
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);  //访问本机地址
	servAdr.sin_port = htons(atoi(argv[2]));

	//连接
	if (connect(hsocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		error_handling("connect() error!\n");
	else
		puts("connected......");
	
	fputs("operand count: ", stdout);
	scanf("%d", &opndCnt);
	opmsg[0] = (char)opndCnt; //1字节整数类型，所以计算的数不能超过1字节整数
	for (int i = 0; i < opndCnt; i++) {
		printf("operand %d：", i+1);
		scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
	}
	fgetc(stdin);  //删除缓冲区中的\n
	fputs("operator: ", stdout);
	scanf("%c", &opmsg[opndCnt * OPSZ + 1]); //不删除\n 此步骤无法执行
	if (send(hsocket, opmsg, opndCnt * OPSZ + 2, 0) == SOCKET_ERROR)
		error_handling("send message error!\n");
	if (recv(hsocket, (char*)&result, 4, 0) == SOCKET_ERROR) //4字节的result
		error_handling("recv result error!\n");
	printf("opration result: %d\n", result);

	closesocket(hsocket);
	WSACleanup();
	return 0;
#endif

}

