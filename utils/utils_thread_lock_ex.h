/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     utils_thread_lock_ex.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-23
*/

#ifndef TWILIGHT_UTILS_THREAD_LOCK_EX_H
#define TWILIGHT_UTILS_THREAD_LOCK_EX_H

#include <Windows.h>

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_UTILS_THREAD_LOCK_EX_API __declspec(dllexport)
#else
#define TWILIGHT_UTILS_THREAD_LOCK_EX_API __declspec(dllimport)
#endif

#define TWILIGHT_UTILS_THREAD_LOCK_EX_CALLMETHOD __stdcall

class TWILIGHT_UTILS_THREAD_LOCK_EX_API utils_thread_lock_ex {
private:
    CRITICAL_SECTION m_csLock;

public:
    utils_thread_lock_ex();
    ~utils_thread_lock_ex();

    CRITICAL_SECTION* get();
    void enter();
    void leave();
};


#endif //TWILIGHT_UTILS_THREAD_LOCK_EX_H
