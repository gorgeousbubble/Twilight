/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     nets_socket.cpp
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-03-02
*/

#include "nets_socket.h"
#include "../utils/utils_safe.h"
#include "../utils/utils_thread_lock.h"
#include <Ws2tcpip.h>
#include <process.h>

using namespace std;

// global variable
char nets_socket::m_pcLocalIP[SOB_IP_LENGTH] = { 0 };
USHORT nets_socket::m_sLocalPort = 0;

//----------------------------------------------
// @Function: initial
// @Purpose: initialize socket
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void nets_socket::initial() {
    WORD wVersionRequested;
    WSADATA wsaData;
    int nErr;
    // request version of socket
    wVersionRequested = MAKEWORD(2, 2);
    // startup WSA environment
    nErr = ::WSAStartup(wVersionRequested, &wsaData);
    if (nErr != 0) {
        return;
    }
}

//----------------------------------------------
// @Function: release
// @Purpose: release socket
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void nets_socket::release() {
    // cleanup WSA environment
    ::WSACleanup();
}

//----------------------------------------------
// @Function: create_tcp_socket
// @Purpose: create tcp socket
// @Since: v1.00a
// @Para: None
// @Return: SOCKET socket
//----------------------------------------------
SOCKET nets_socket::create_tcp_socket() {
    // create tcp socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        s = NULL;
        m_nLastWSAError = WSAGetLastError();
    }
    // create asynchronous event(write and read)
    if (m_SocketWriteEvent == NULL) {
        m_SocketWriteEvent = WSACreateEvent();
    }
    if (m_SocketReadEvent == NULL) {
        m_SocketReadEvent = WSACreateEvent();
    }
    // set socket property
    const char chOpt = 1;
    int nRet = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));
    return s;
}

//----------------------------------------------
// @Function: create_udp_socket
// @Purpose: create udp socket
// @Since: v1.00a
// @Para: None
// @Return: SOCKET socket
//----------------------------------------------
SOCKET nets_socket::create_udp_socket() {
    // create udp socket
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET) {
        s = NULL;
        m_nLastWSAError = WSAGetLastError();
    }
    // create asynchronous event(write and read)
    if (m_SocketWriteEvent == NULL) {
        m_SocketWriteEvent = WSACreateEvent();
    }
    if (m_SocketReadEvent == NULL) {
        m_SocketReadEvent = WSACreateEvent();
    }
    return s;
}

//----------------------------------------------
// @Function: nets_socket
// @Purpose: construct function
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
nets_socket::nets_socket() {
    // initial value
    m_socket = NULL;
    m_SocketWriteEvent = NULL;
    m_SocketReadEvent = NULL;
    // clean flags
    m_bIsConnected = false;
    m_sMaxCount = SOB_DEFAULT_MAX_CLIENT;
    // clean count
    m_nAcceptCount = 0;
    m_mapAccept.clear();
    // clean array
    memset(m_pcRemoteIP, 0, SOB_IP_LENGTH);
    m_sRemotePort = 0;
    memset(m_pcHostIP, 0, SOB_IP_LENGTH);
    m_sHostPort = 0;
}

//----------------------------------------------
// @Function: ~nets_socket
// @Purpose: destruct function
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
nets_socket::~nets_socket() {
    // close the socket
    if (m_socket) {
        closesocket(m_socket);
        m_socket = NULL;
    }
    // close socket event
    if (m_SocketWriteEvent) {
        WSACloseEvent(m_SocketWriteEvent);
        m_SocketWriteEvent = NULL;
    }
    if (m_SocketReadEvent) {
        WSACloseEvent(m_SocketReadEvent);
        m_SocketReadEvent = NULL;
    }
}

//----------------------------------------------
// @Function: set_recv_timeout
// @Purpose: set receive timeout property
// @Since: v1.00a
// @Para: UINT uiMSec // timeout value
// @Return: None
//----------------------------------------------
void nets_socket::set_recv_timeout(UINT uiMSec) {
    UINT uiMseTimeOut = uiMSec;
    ::setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

//----------------------------------------------
// @Function: set_send_timeout
// @Purpose: set send timeout property
// @Since: v1.00a
// @Para: UINT uiMSec // timeout value
// @Return: None
//----------------------------------------------
void nets_socket::set_send_timeout(UINT uiMSec) {
    UINT uiMseTimeOut = uiMSec;
    ::setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

//----------------------------------------------
// @Function: set_send_buffer_size
// @Purpose: set send buffer size property
// @Since: v1.00a
// @Para: UINT uiByte // buffer size
// @Return: None
//----------------------------------------------
void nets_socket::set_send_buffer_size(UINT uiByte) {
    UINT uiBufferSize = uiByte;
    ::setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));
}

//----------------------------------------------
// @Function: set_recv_buffer_size
// @Purpose: set receive buffer size property
// @Since: v1.00a
// @Para: UINT uiByte // buffer size
// @Return: None
//----------------------------------------------
void nets_socket::set_recv_buffer_size(UINT uiByte) {
    UINT uiBufferSize = uiByte;
    ::setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));
}
