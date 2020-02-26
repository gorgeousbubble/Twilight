/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     nets_serial.cpp
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-26
*/

#include "nets_serial.h"

//----------------------------------------------
// @Function: nets_serial
// @Purpose: construct function
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
nets_serial::nets_serial() {
    // initial value
    m_hCom = INVALID_HANDLE_VALUE;
    m_hListen = INVALID_HANDLE_VALUE;
    m_bOpen = false;
    m_bRecv = false;
    // clean count
    m_dwSendCount = 0;
    m_dwRecvCount = 0;
    // clean memory
    memset(&m_ovWrite, 0, sizeof(m_ovWrite));
    memset(&m_ovRead, 0, sizeof(m_ovRead));
    memset(&m_ovWait, 0, sizeof(m_ovWait));
    memset(m_chSendBuf, 0, sizeof(m_chSendBuf));
    memset(m_chRecvBuf, 0, sizeof(m_chRecvBuf));
    // initial critical section
    ::InitializeCriticalSection(&m_csComSync);
}

//----------------------------------------------
// @Function: ~nets_serial
// @Purpose: destruct function
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
nets_serial::~nets_serial() {
    // close open flag
    ::EnterCriticalSection(&m_csComSync);
    m_bOpen = false;
    ::LeaveCriticalSection(&m_csComSync);
    // clean event flags
    if (m_hCom != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_hCom);
        m_hCom = INVALID_HANDLE_VALUE;
    }
    if (m_ovWrite.hEvent != NULL) {
        ::CloseHandle(m_ovWrite.hEvent);
        m_ovWrite.hEvent = NULL;
    }
    if (m_ovRead.hEvent != NULL) {
        ::CloseHandle(m_ovRead.hEvent);
        m_ovRead.hEvent = NULL;
    }
    if (m_ovWait.hEvent != NULL) {
        ::CloseHandle(m_ovWait.hEvent);
        m_ovWait.hEvent = NULL;
    }
    if (m_hListen != INVALID_HANDLE_VALUE) {
        ::WaitForSingleObject(m_hListen, INFINITE);
        ::CloseHandle(m_hListen);
        m_hListen = INVALID_HANDLE_VALUE;
    }
    // delete critical section
    ::DeleteCriticalSection(&m_csComSync);
}
