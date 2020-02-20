/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     logs_simple.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-18
*/

#include "logs_simple.h"

using namespace std;

void logs_simple::get_path(char *str, int size) {
    char chPath[MAX_PATH] = {0};
    char chName[MAX_PATH] = {0};
    char* pTemp = NULL;
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get module file path
    ::GetModuleFileNameA(NULL, chPath, sizeof(chPath));
    // get file name
    pTemp = strrchr(chPath, '\\');
    if (pTemp != NULL) {
        strcat_s(chName, pTemp + 1);
        *pTemp = '\0';
    }
    // split file name without suffix
    for (pTemp = strrchr(chName, '.'); pTemp != chName + MAX_PATH - 1; ++pTemp) {
        *pTemp = '\0';
    }
    // combine new log path
    char chTemp[MAX_PATH] = {0};
    sprintf_s(chTemp, sizeof(chTemp), "\\log\\%s_%d_%02d_%02d.log", chName, st.wYear, st.wMonth, st.wDay);
    strcat_s(chPath, chTemp);
    // copy to dest array
    memset(str, 0, size);
    memcpy_s(str, size, chPath, strlen(chPath));
}

void logs_simple::create() {
    char chPath[MAX_PATH] = {0};
    char chName[MAX_PATH] = {0};
    char* pTemp = NULL;
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get module file path
    ::GetModuleFileNameA(NULL, chPath, sizeof(chPath));
    // get file name
    pTemp = strrchr(chPath, '\\');
    if (pTemp != NULL) {
        strcat_s(chName, pTemp + 1);
        *pTemp = '\0';
    }
    // split file name without suffix
    for (pTemp = strrchr(chName, '.'); pTemp != chName + MAX_PATH - 1; ++pTemp) {
        *pTemp = '\0';
    }
    // check log dir exist(if not create it...)
    char chDir[MAX_PATH] = {0};
    sprintf_s(chDir, sizeof(chDir), "%s\\log", chPath);
    if (!::PathFileExistsA(chDir)) {
        ::CreateDirectoryA(chDir, NULL);
    }
    // combine new log path
    char chTemp[MAX_PATH] = {0};
    sprintf_s(chTemp, sizeof(chTemp), "\\log\\%s_%d_%02d_%02d.log", chName, st.wYear, st.wMonth, st.wDay);
    strcat_s(chPath, chTemp);
    // create log file
    ofstream fout(chPath, ios::app);
    fout.close();
}

void logs_simple::write(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
    va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
    va_end(arg_ptr);
    fout.close();
}

void logs_simple::debug(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d -DEBUG- [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
            va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
            va_end(arg_ptr);
    fout.close();
}

void logs_simple::info(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d -INFO- [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
            va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
            va_end(arg_ptr);
    fout.close();
}

void logs_simple::event(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d -EVENT- [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
            va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
            va_end(arg_ptr);
    fout.close();
}

void logs_simple::warning(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d -WARNING- [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
            va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
            va_end(arg_ptr);
    fout.close();
}

void logs_simple::error(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d -ERROR- [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
            va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
            va_end(arg_ptr);
    fout.close();
}

void logs_simple::critical(LPCSTR str, ...) {
    char chPath[MAX_PATH] = {0};
    char chLog[MAX_PATH] = {0};
    char chParas[MAX_PATH] = {0};
    // get system time now
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    // get process id
    DWORD dwProcess;
    dwProcess = ::GetCurrentProcessId();
    // get thread id
    DWORD dwThread;
    dwThread = ::GetCurrentThreadId();
    // get log file path
    get_path(chPath, sizeof(chPath));
    if (!::PathFileExistsA(chPath)) {
        create();
    }
    // record to log file
    ofstream fout(chPath, ios::app);
    fout.seekp(ios::end);
    // print prefix info
    memset(chLog, 0, sizeof(chLog));
    _snprintf_s(chLog, sizeof(chLog), "%d/%02d/%02d %02d:%02d:%02d -CRITICAL- [%d](%d):", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, dwProcess, dwThread);
    fout << chLog;
    // print parameters
    va_list arg_ptr;
            va_start(arg_ptr, str);
    memset(chParas, 0, sizeof(chParas));
    _vsnprintf(chParas, sizeof(chParas), str, arg_ptr);
    fout << chParas << endl;
            va_end(arg_ptr);
    fout.close();
}
