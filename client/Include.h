#ifndef INCLUDE_H_
#define INCLUDE_H_

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#ifdef _WIN32

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
#include <windows.h>
#endif


#ifdef __linux__


#define closesocket(A) close(A)
#define __min(a,b) (((a) < (b)) ? (a) : (b))

#define SOCKET_ERROR -1
#define INVALID_SOCKET ~0

//#define ioctlsocket ioctl

typedef int SOCKET;
#define sockaddr_in SOCKADDR_IN
//#define WSAGetLastError() errno
#include <unistd.h>
#include <arpa/inet.h>

//typedef uint32_t DWORD;
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define Sleep(x) usleep((x)*1000)

#endif


#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <vector>
#include <string.h>

const int port = 8400;
const int64_t size_100mb = 104857600;
const int Chunk_Buffer_Size = 4096;
const int Chunk_RecFile_Size = 65536;




#endif // !INCLUDE_H_



