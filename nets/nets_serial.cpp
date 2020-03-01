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
    ls = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey);
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
            ls2 = ::RegEnumValueW(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize);
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

//----------------------------------------------
// @Function: init
// @Purpose: init serial port
// @Since: v1.00a
// @Para: S_NETS_SERIAL_PROPERTY sCommProperty serial port property
// @Return: None
//----------------------------------------------
bool nets_serial::init(S_NETS_SERIAL_PROPERTY sCommProperty) {
    utils_thread_lock lock(&m_csComSync);
    bool bRet = false;
    // create serial port
    bRet = create_serial(sCommProperty.chPort);
    if (!bRet) {
        return false;
    }
    // configure serial port
    bRet = conf_serial(sCommProperty);
    if (!bRet) {
        return false;
    }
    // set open flag
    m_bOpen = true;
    return true;
}

//----------------------------------------------
// @Function: init_listen
// @Purpose: init serial port listen
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
bool nets_serial::init_listen() {
    // check listen handle
    if (m_hListen != INVALID_HANDLE_VALUE) {
        return false;
    }
    // create thread for receive buffer
    unsigned int uThreadID;
    m_hListen = (HANDLE)::_beginthreadex(NULL, 0, (_beginthreadex_proc_type)on_receive_buffer, this, 0, &uThreadID);
    if (!m_hListen) {
        return false;
    }
    // set thread priority
    BOOL bRet = FALSE;
    bRet = ::SetThreadPriority(m_hListen, THREAD_PRIORITY_ABOVE_NORMAL);
    if (!bRet) {
        return false;
    }
    return true;
}

//----------------------------------------------
// @Function: on_receive_buffer
// @Purpose: handle receive serial port thread
// @Since: v1.00a
// @Para: LPVOID lpParameters // send parameters
// @Return: unsigned int // thread exit code
//----------------------------------------------
unsigned int nets_serial::on_receive_buffer(LPVOID lpParameters) {
    nets_serial* pBase = reinterpret_cast<nets_serial*>(lpParameters);
    BOOL bStatus = FALSE;
    DWORD dwWaitEvent = 0;
    DWORD dwBytes = 0;
    DWORD dwError = 0;
    COMSTAT cs = { 0 };
    BYTE chReadBuf[NETS_SERIAL_BUFFER_SIZE] = { 0 };

    while (true) {
        // check serial port open status
        EnterCriticalSection(&pBase->m_csComSync);
        if (!pBase->m_bOpen) {
            LeaveCriticalSection(&pBase->m_csComSync);
            break;
        }
        LeaveCriticalSection(&pBase->m_csComSync);

        dwWaitEvent = 0;
        pBase->m_ovWait.Offset = 0;
        // wait comm event...
        bStatus = ::WaitCommEvent(pBase->m_hCom, &dwWaitEvent, &pBase->m_ovWait);
        if (FALSE == bStatus && GetLastError() == ERROR_IO_PENDING) {
            bStatus = ::GetOverlappedResult(pBase->m_hCom, &pBase->m_ovWait, &dwBytes, TRUE);
        }
        // clear comm error
        ClearCommError(pBase->m_hCom, &dwError, &cs);
        // receive data from serial port
        if (TRUE == bStatus && (dwWaitEvent & EV_RXCHAR) && cs.cbInQue > 0) {
            dwBytes = 0;
            pBase->m_ovRead.Offset = 0;
            // clear receive buffer
            memset(chReadBuf, 0, sizeof(chReadBuf));
            bStatus = ReadFile(pBase->m_hCom, chReadBuf, sizeof(chReadBuf), &dwBytes, &pBase->m_ovRead);
            PurgeComm(pBase->m_hCom, PURGE_RXCLEAR | PURGE_RXABORT);
            // read data buffer
            EnterCriticalSection(&pBase->m_csComSync);
            pBase->m_dwRecvCount = dwBytes;
            memset(pBase->m_chRecvBuf, 0, sizeof(pBase->m_chRecvBuf));
            memcpy_s(pBase->m_chRecvBuf, sizeof(pBase->m_chRecvBuf), chReadBuf, sizeof(chReadBuf));
            pBase->m_bRecv = true;
            LeaveCriticalSection(&pBase->m_csComSync);
        }
    }
    return 0;
}

//----------------------------------------------
// @Function: close
// @Purpose: close serial port
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void nets_serial::close() {
    // set open status
    EnterCriticalSection(&m_csComSync);
    m_bOpen = false;
    LeaveCriticalSection(&m_csComSync);
    // close handle
    if (m_hCom != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_hCom);
        m_hCom = INVALID_HANDLE_VALUE;
    }
    // clean environment
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
}

//----------------------------------------------
// @Function: close_listen
// @Purpose: close serial port listen
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void nets_serial::close_listen() {
    // close listen thread...
    if (INVALID_HANDLE_VALUE != m_hListen) {
        ::WaitForSingleObject(m_hListen, INFINITE);
        ::CloseHandle(m_hListen);
        m_hListen = INVALID_HANDLE_VALUE;
    }
}

//----------------------------------------------
// @Function: set_send_buffer
// @Purpose: set send buffer for serial port
// @Since: v1.00a
// @Para: UCHAR *pBuff // send buffer address
// @Para: int nSize // send buffer size
// @Para: DWORD &dwSendCount // real send count
// @Return: None
//----------------------------------------------
void nets_serial::set_send_buffer(UCHAR *pBuff, int nSize, DWORD &dwSendCount) {
    utils_thread_lock lock(&m_csComSync);
    m_dwSendCount = dwSendCount;
    memcpy_s(m_chSendBuf, sizeof(m_chSendBuf), pBuff, nSize);
}

//----------------------------------------------
// @Function: get_recv_buffer
// @Purpose: get recv buffer for serial port
// @Since: v1.00a
// @Para: UCHAR *pBuff // recv buffer address
// @Para: int nSize // recv buffer size
// @Para: DWORD &dwRecvCount // real recv count
// @Return: None
//----------------------------------------------
void nets_serial::get_recv_buffer(UCHAR *pBuff, int nSize, DWORD &dwRecvCount) {
    utils_thread_lock lock(&m_csComSync);
    dwRecvCount = m_dwRecvCount;
    memcpy_s(pBuff, nSize, m_chRecvBuf, sizeof(m_chRecvBuf));
}

//----------------------------------------------
// @Function: open_port
// @Purpose: open the serial port
// @Since: v1.00a
// @Para: S_NETS_SERIAL_PROPERTY sCommProperty // serial port property
// @Return: None
//----------------------------------------------
bool nets_serial::open_port(S_NETS_SERIAL_PROPERTY sCommProperty) {
    bool bRet = false;
    // initialize serial port
    bRet = init(sCommProperty);
    if (!bRet) {
        return false;
    }
    // initialize listen
    bRet = init_listen();
    if (!bRet) {
        return false;
    }
    return true;
}

//----------------------------------------------
// @Function: close_port
// @Purpose: close the serial port
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void nets_serial::close_port() {
    close();
    close_listen();
}

//----------------------------------------------
// @Function: on_translate_buffer
// @Purpose: translate send buffer thread
// @Since: v1.00a
// @Para: None
// @Return: bool (true/false)
//----------------------------------------------
bool nets_serial::on_translate_buffer() {
    BOOL bStatus = FALSE;
    DWORD dwError = 0;
    COMSTAT cs = { 0 };
    DWORD dwBytes = 0;
    BYTE chSendBuf[NETS_SERIAL_BUFFER_SIZE] = { 0 };
    // clean the serial port
    // ClearCommError(m_hCOM, &dwError, &cs);
    PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_TXABORT);
    m_ovWrite.Offset = 0;
    // clean the send buffer
    EnterCriticalSection(&m_csComSync);
    memset(chSendBuf, 0, sizeof(chSendBuf));
    memcpy_s(chSendBuf, sizeof(chSendBuf), m_chSendBuf, sizeof(m_chSendBuf));
    LeaveCriticalSection(&m_csComSync);
    // write date to serial port
    bStatus = ::WriteFile(m_hCom, chSendBuf, m_dwSendCount, &dwBytes, &m_ovWrite);
    if (FALSE == bStatus && GetLastError() == ERROR_IO_PENDING) {
        if (FALSE == ::GetOverlappedResult(m_hCom, &m_ovWrite, &dwBytes, TRUE)) {
            return false;
        }
    }
    return true;
}
