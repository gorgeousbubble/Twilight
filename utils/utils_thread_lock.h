/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     utils_thread_lock.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-23
*/

#ifndef TWILIGHT_UTILS_THREAD_LOCK_H
#define TWILIGHT_UTILS_THREAD_LOCK_H

#include <Windows.h>

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_UTILS_THREAD_LOCK_API __declspec(dllexport)
#else
#define TWILIGHT_UTILS_THREAD_LOCK_API __declspec(dllimport)
#endif

#define TWILIGHT_UTILS_THREAD_LOCK_CALLMETHOD __stdcall

class utils_thread_lock {
private:
    CRITICAL_SECTION* m_pLock;
    bool m_bSafe;

public:
    explicit utils_thread_lock(const CRITICAL_SECTION* lock, const bool safe= true);
    ~utils_thread_lock();
};


#endif //TWILIGHT_UTILS_THREAD_LOCK_H
