/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     nets_socket.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-03-02
*/

#ifndef TWILIGHT_NETS_SOCKET_H
#define TWILIGHT_NETS_SOCKET_H

#include <WinSock2.h>

#include <iostream>
#include <vector>
#include <map>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_NETS_SOCKET_API __declspec(dllexport)
#else
#define TWILIGHT_NETS_SOCKET_API __declspec(dllimport)
#endif

#define TWILIGHT_NETS_SOCKET_CALLMETHOD __stdcall

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#define _WINSOCKAPI_

#define SOB_IP_LENGTH				128

#define SOB_TCP_SEND_BUFFER			(32 * 1024)
#define SOB_TCP_RECV_BUFFER			(32 * 1024)
#define SOB_UDP_RECV_BUFFER			(32 * 1024)

#define SOB_DEFAULT_TIMEOUT_SEC		5
#define SOB_DEFAULT_MAX_CLIENT		10

#define SOB_RET_OK					1
#define SOB_RET_FAIL				0
#define SOB_RET_TIMEOUT				-1
#define SOB_RET_CLOSE				-2

typedef struct {
    SOCKET Socket;
    SOCKADDR_IN SocketAddr;
} S_NETS_SOCKET_PROPERTY, *LPS_NETS_SOCKET_PROPERTY;

class TWILIGHT_NETS_SOCKET_API nets_socket {
private:
    SOCKET m_socket;
    WSAEVENT m_SocketWriteEvent;
    WSAEVENT m_SocketReadEvent;

    int m_nLastWSAError;

public:
    static void initial();
    static void release();

    SOCKET create_tcp_socket();
    SOCKET create_udp_socket();
};


#endif //TWILIGHT_NETS_SOCKET_H
