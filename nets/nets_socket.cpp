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
