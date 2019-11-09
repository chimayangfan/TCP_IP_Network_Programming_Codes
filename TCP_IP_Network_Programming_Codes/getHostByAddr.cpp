//UDP迭代回声服务器端 / 客户端：
//服务器端在同一时刻只与一个客户端相连， 并提供回声服务。
//服务器端依次向5个客户端提供服务并退出。
//客户端接收用户输人的字符串并发送到服务器端。
//服务器端将接收的字符串数据传回客户端，即“回声”。
//服务器端与客户端之间的字符串回声一直执行到客户端输人Q为止。

#include"syscheck.h"

#ifdef __linux
#include <netdb.h>
#endif

void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
#ifdef __linux
	int i;
	struct hostent *host;
	struct sockaddr_in addr;
	if (argc != 2) {
		printf("Usage : %s <addr> \n", argv[0]);
		exit(1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
	if (!host) {
		error_handling("gethostbyaddr error");
	}
	printf("Official name: %s \n", host->h_name);
	for (i = 0; host->h_aliases[i]; i++) {
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
	}
	printf("Address type : %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
	for (i = 0; host->h_addr_list[i]; i++) {
		printf("IP addr %d:%s \n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	return 0;
#endif

#ifdef _WIN32

#endif

}

