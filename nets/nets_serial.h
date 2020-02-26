/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     nets_serial.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-26
*/

#ifndef TWILIGHT_NETS_SERIAL_H
#define TWILIGHT_NETS_SERIAL_H

#include <Windows.h>

#include <iostream>
#include <map>
#include <vector>
#include <process.h>

#pragma comment(lib, "WinMM.lib")

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_NETS_SERIAL_API __declspec(dllexport)
#else
#define TWILIGHT_NETS_SERIAL_API __declspec(dllimport)
#endif

#define TWILIGHT_NETS_SERIAL_CALLMETHOD __stdcall

#define NETS_SERIAL_BUFFER_SIZE 4096

class TWILIGHT_NETS_SERIAL_API nets_serial {
private:
    HANDLE m_hCom;
    HANDLE m_hListen;

    OVERLAPPED m_ovWrite;
    OVERLAPPED m_ovRead;
    OVERLAPPED m_ovWait;

public:
    CRITICAL_SECTION m_csComSync;
    volatile bool m_bOpen;
    volatile bool m_bRecv;
    std::map<int, std::string> m_mapCom;

    UCHAR m_chSendBuf[NETS_SERIAL_BUFFER_SIZE];
    UCHAR m_chRecvBuf[NETS_SERIAL_BUFFER_SIZE];
    DWORD m_dwSendCount;
    DWORD m_dwRecvCount;

    nets_serial();
    ~nets_serial();

};


#endif //TWILIGHT_NETS_SERIAL_H
