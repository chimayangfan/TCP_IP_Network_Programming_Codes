//�շ��ļ��������� / �ͻ��ˣ�
//�ͻ��˽����û�����Ĵ����ļ���
//�ͻ�������������˴�����ļ�����ָ�ļ�
//���ָ���ļ����ڣ��������˾ͽ��䷢�͸��ͻ��ˣ���֮����Ͽ�����

#include"syscheck.h"
#include <iostream>
#include <string>

#define BUF_SIZE	1024

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef __linux
	int serv_sock, clnt_sock;
	char buffer[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_sz;

	if (argc != 2)
	{
		printf("Usage:%s<port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 1) == -1)
		error_handling("listen() error");

	clnt_addr_sz = sizeof(clnt_addr);

	for (size_t cnt = 0; true; ++cnt) {
		// get client socket
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
	
		// get the filename
		memset(buffer, 0, BUF_SIZE);
		if ((recv(clnt_sock, buffer, BUF_SIZE, 0)) < 0) {
			error_handling("failed to filename data!");
			break;
		}

		// start transmission
		std::string filename(buffer);
		FILE *fp = fopen(filename.c_str(), "r");
		if (fp == NULL)
			printf("NO_SUCH_FILE_REQUEST \n");
		else {
			memset(buffer, 0, BUF_SIZE);
			int sd_sz;  // size of data to send
			while ((sd_sz = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
				if (send(clnt_sock, buffer, sd_sz, 0) < 0) {  // see if failed
					printf("SEND_FILE_FAILED \n");
					break;
				}
				memset(buffer, 0, BUF_SIZE);
			}
			fclose(fp);
}
		close(clnt_sock);
		printf("clnt_sock closed (seq: %d) \n", cnt);
	}
	close(serv_sock);
	return 0;
#endif

#ifdef _WIN32

#endif

}

