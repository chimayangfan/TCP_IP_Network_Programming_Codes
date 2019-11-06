//收发文件服务器端 / 客户端：
//客户端接受用户输入的传输文件名
//客户端请求服务器端传输该文件名所指文件
//如果指定文件存在，服务器端就将其发送给客户端；反之，则断开连接

#include"syscheck.h"
#include <iostream>
#include <string>
#include <regex>

#define BUF_SIZE	1024

int filename_check(const std::string &filename) {
	// check quit
	if (std::regex_match(filename, std::regex("[Qq]uit"))) {
		std::cout << "Client closed....." << std::endl;
		return 1;
	}
	// check illegal filename
	if (std::regex_match(filename, std::regex(".*(([\\.]{2})|([/])).*"))) {
		std::cout << "illegal filename!" << std::endl;
		return 2;
	}
	return 0;
}

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef __linux
	int sock;
	char buffer[BUF_SIZE];
	std::string str_msg;
	int str_len;
	struct sockaddr_in serv_addr;

	if (argc != 3) {
		std::cout << "usage : " << argv[0] << " <IP> <port>" << std::endl;
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	std::cout << "Connected......" << std::endl;
	std::cout << "Use 'quit' to finish the download" << std::endl;

	// get filename
	std::cout << "Input filename: " << std::ends;
	getline(std::cin, str_msg);
	// ATTENTION: these check should have been implemented in the server
	if (filename_check(str_msg) != 0)
		return 0;

	// check 
	FILE *fp = fopen(str_msg.c_str(), "w");
	if (fp == NULL)
		error_handling("Can't open/create the file!");

	if ((send(sock, str_msg.c_str(), str_msg.length(), 0)) < 0)
		error_handling("Send filename failed");
	std::cout << "filename sent..." << std::endl;

	int recv_sz;
	while ((recv_sz = recv(sock, buffer, BUF_SIZE - 1, 0)) > 0) {    // receive the data
		if (fwrite(buffer, recv_sz, 1, fp) < 0) {    // write to a file
			std::cout << "Failed to write the file!" << std::endl;
			break;
		}
		memset(buffer, 0, BUF_SIZE);
	}
	std::cout << "Download finished..." << std::endl;
	fclose(fp);
	close(sock);
	return 0;
}

#endif

#ifdef _WIN32

#endif

}

