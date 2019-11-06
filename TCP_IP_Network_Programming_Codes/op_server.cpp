//�������������� / �ͻ��ˣ�
//�ͻ������ӷ������˺���1�ֽ�������ʽ���ݴ������ָ�����
//�ͻ�����������˴��ݵ�ÿ������������ռ��4�ֽ�
//�������������ݺ���Ŵ�����������������Ϣռ��1�ֽ�
//ѡ���ַ� + -*֮һ����
//����������4�ֽ���������ͻ��˴�������Ľ��
//�ͻ��˵õ�������������ֹ��������˵�����

#include"syscheck.h"

#define BUF_SIZE 	1024
#define OPSZ	4
#define MAX_CONNECT_SIZE 5//���ͻ���������

int calculate(int opnum, int opnds[], char op) {
	int result = opnds[0], i;
	switch (op) {
	case '+':
		for (i = 1; i<opnum; i++) result += opnds[i];
		break;
	case '-':
		for (i = 1; i<opnum; i++) result -= opnds[i];
		break;
	case '*':
		for (i = 1; i<opnum; i++) result *= opnds[i];
		break;
	}
	return result;
}

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef linux
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_sz;
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
	if (listen(serv_sock, MAX_CONNECT_SIZE) == -1) {
		error_handling("listen() error");
	}

	clnt_addr_sz = sizeof(clnt_addr);

	for (i = 0; i<MAX_CONNECT_SIZE; i++) {
		opnd_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
		read(clnt_sock, &opnd_cnt, 1);

		recv_len = 0;
		while ((opnd_cnt*OPSZ + 1)>recv_len) {
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		result = calculate(opnd_cnt, (int *)opinfo, opinfo[recv_len - 1]);
		write(clnt_sock, (char *)&result, sizeof(result));

		close(clnt_sock);
	}
	close(serv_sock);

	return 0;

#endif

#ifdef _WIN32
	WSADATA wsaData;
	SOCKET hClntSock, hServSock;
	char opinfo[BUF_SIZE];
	int opndcnt, i, result;
	int recvlen, recvcnt;
	SOCKADDR_IN clntAdr, servAdr;
	int clntAdrSize;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	//winsock���ʼ��
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);//�������������׽��֣�IPV4  �������Ӵ��䣩 
	if (hServSock == INVALID_SOCKET) 
		error_handling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));   //���
	servAdr.sin_family = AF_INET; //IPv4
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);  //��ȡ������ַ
	servAdr.sin_port = htons(atoi(argv[1]));
	//�󶨵�ַ
	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		error_handling("bind() error");
	//�������������ͻ������ȴ���ĿΪ5
	if (listen(hServSock, MAX_CONNECT_SIZE) == SOCKET_ERROR)
		error_handling("listen() error");
	//���ܿͻ���
	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i < MAX_CONNECT_SIZE; i++) {
		opndcnt = 0;
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		recv(hClntSock, (char*)&opndcnt, 1, 0);
		recvlen = 0;
		while ((opndcnt * OPSZ + 1)>recvlen) {
			recvcnt = recv(hClntSock, &opinfo[recvlen], BUF_SIZE - 1, 0);
			recvlen += recvcnt;
		}
		result = calculate(opndcnt, (int*)opinfo, opinfo[recvlen - 1]);
		if (send(hClntSock, (char*)&result, 4, 0) == SOCKET_ERROR)
			error_handling("send result error");
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
#endif

}

