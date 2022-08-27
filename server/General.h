//#pragma once
#ifndef GENERAL_H_
#define GENERAL_H_


#ifdef _WIN32

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
typedef int size_sockaddr;

#endif


#ifdef __linux__

#define __min(a,b) (((a) < (b)) ? (a) : (b))

#define SOCKET_ERROR -1
#define INVALID_SOCKET ~0

#define closesocket(A) close(A)

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
typedef unsigned int size_sockaddr;

#endif

#include <filesystem>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <string.h>

const int port = 8400;
const int Chunk_Buffer_Size = 4096;
const int Chunk_RecFile_Size = 65536;

#endif // !GENERAL_H_