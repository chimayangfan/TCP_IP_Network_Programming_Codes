#pragma once
#ifdef __linux
	#include<unistd.h>//unix ��׼���������
	#include<stdlib.h>//standard library��׼��ͷ�ļ�
	#include<stdio.h>//��׼����������ͷ�ļ�
	#include<string.h>
	#include<sys/socket.h>//unix socket���ͷ�ļ�
	#include<arpa/inet.h>//unix socket���ͷ�ļ�
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

