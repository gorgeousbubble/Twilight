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

nets_socket::nets_socket() {

}

nets_socket::~nets_socket() {

}
