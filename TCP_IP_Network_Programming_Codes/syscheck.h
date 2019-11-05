#pragma once
#ifdef linux
	#include<unistd.h>
	#include<stdlib.h>
	#include<stdio.h>
	#include<string.h>
	#include<sys/socket.h>
	#include<arpa/inet.h>
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

