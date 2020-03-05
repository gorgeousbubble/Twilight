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

class nets_socket {

};


#endif //TWILIGHT_NETS_SOCKET_H
