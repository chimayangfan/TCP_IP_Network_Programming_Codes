#pragma once
#ifdef __linux
	#include<unistd.h>//unix 标准的输入输出
	#include<stdlib.h>//standard library标准库头文件
	#include<stdio.h>//标准输入输出库的头文件
	#include<string.h>
	#include<sys/socket.h>//unix socket编程头文件
	#include<arpa/inet.h>//unix socket编程头文件
#endif

#ifdef _UNIX


#endif

#ifdef __WINDOWS_
	no_os_flag = 0;
	cout << "It is in Windows OS!" << endl;
#endif

#ifdef _WIN32
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<winsock2.h>

	#pragma comment(lib, "ws2_32.lib")

#endif

