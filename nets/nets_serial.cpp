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
#include "../utils/utils_safe.h"
#include "../utils/utils_thread_lock.h"

using namespace std;

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

//----------------------------------------------
// @Function: get_status
// @Purpose: get serial port open status
// @Since: v1.00a
// @Para: None
// @Return: bool (true:open/false:close)
//----------------------------------------------
bool nets_serial::get_status() const {
    utils_thread_lock lock(&m_csComSync);
    return m_bOpen;
}

//----------------------------------------------
// @Function: get_recv
// @Purpose: get serial port receive flag
// @Since: v1.00a
// @Para: None
// @Return: bool (true/false)
//----------------------------------------------
bool nets_serial::get_recv() const {
    utils_thread_lock lock(&m_csComSync);
    return m_bRecv;
}

//----------------------------------------------
// @Function: set_recv
// @Purpose: set serial port receive flag
// @Since: v1.00a
// @Para: bool recv receive flag
// @Return: None
//----------------------------------------------
void nets_serial::set_recv(bool recv) {
    utils_thread_lock lock(&m_csComSync);
    m_bRecv = recv;
}

//----------------------------------------------
// @Function: enum_serial
// @Purpose: enum serial ports
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void nets_serial::enum_serial() {
    HKEY hKey;
    LSTATUS ls;
    ls = RegOpenKeyExW(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey);
    if (ls == ERROR_SUCCESS) {
        TCHAR szPortName[256] = { 0 };
        TCHAR szComName[256] = { 0 };
        DWORD dwLong = 0;
        DWORD dwSize = 0;
        int nCount = 0;
        // clear map...
        m_mapCom.clear();
        while (true) {
            LSTATUS ls2;
            // enum value register
            dwLong = dwSize = 256;
            ls2 = RegEnumValueW(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize);
            if (ERROR_NO_MORE_ITEMS == ls2) {
                break;
            }
            // string convention
            int iLen = WideCharToMultiByte(CP_ACP, 0, szComName, -1, NULL, 0, NULL, NULL);
            char* chRtn = new char[iLen + 1];
            memset(chRtn, 0, iLen + 1);
            WideCharToMultiByte(CP_ACP, 0, szComName, -1, chRtn, iLen, NULL, NULL);
            // insert into map
            string str(chRtn);
            m_mapCom.insert(pair<int, string>(nCount, str));
            safe_delete_array(chRtn);
            nCount++;
        }
    }
}

//----------------------------------------------
// @Function: create_serial
// @Purpose: create serial port
// @Since: v1.00a
// @Para: const char* szPort serial port name
// @Return: None
//----------------------------------------------
bool nets_serial::create_serial(const char* szPort) {
    utils_thread_lock lock(&m_csComSync);
    DWORD dwError;
    // create serial port by name
    m_hCom = ::CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    if (INVALID_HANDLE_VALUE == m_hCom) {
        dwError = GetLastError();
        return false;
    }
    return true;
}

//----------------------------------------------
// @Function: conf_serial
// @Purpose: configure serial port
// @Since: v1.00a
// @Para: S_NETS_SERIAL_PROPERTY sCommProperty serial port property
// @Return: None
//----------------------------------------------
bool nets_serial::conf_serial(S_NETS_SERIAL_PROPERTY sCommProperty) {
    utils_thread_lock lock(&m_csComSync);
    BOOL bRet = FALSE;
    // set input and output buffer
    bRet = SetupComm(m_hCom, NETS_SERIAL_BUFFER_SIZE, NETS_SERIAL_BUFFER_SIZE);
    if (!bRet) {
        return false;
    }
    // set DCB structure
    DCB dcb = { 0 };
    bRet = GetCommState(m_hCom, &dcb);
    if (!bRet) {
        return false;
    }
    // set value
    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = sCommProperty.dwBaudRate;
    dcb.ByteSize = sCommProperty.byDataBits;
    dcb.StopBits = sCommProperty.byStopBits;
    dcb.Parity = sCommProperty.byCheckBits;
    bRet = SetCommState(m_hCom, &dcb);
    if (!bRet) {
        return false;
    }
    // set serial port timeout value
    COMMTIMEOUTS ct = { 0 };
    ct.ReadIntervalTimeout = MAXDWORD;
    ct.ReadTotalTimeoutMultiplier = 0;
    ct.ReadTotalTimeoutConstant = 0;
    ct.WriteTotalTimeoutMultiplier = 500;
    ct.WriteTotalTimeoutConstant = 5000;
    bRet = SetCommTimeouts(m_hCom, &ct);
    if (!bRet) {
        return false;
    }
    // clear serial port buffer
    bRet = PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    if (!bRet) {
        return false;
    }
    // create event object
    m_ovRead.hEvent = CreateEvent(NULL, false, false, NULL);
    m_ovWrite.hEvent = CreateEvent(NULL, false, false, NULL);
    m_ovWait.hEvent = CreateEvent(NULL, false, false, NULL);
    // set serial port mask
    SetCommMask(m_hCom, EV_ERR | EV_RXCHAR);
    return true;
}
