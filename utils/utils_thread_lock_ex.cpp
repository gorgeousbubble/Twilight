/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     utils_thread_lock_ex.cpp
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-23
*/

#include "utils_thread_lock_ex.h"

utils_thread_lock_ex::utils_thread_lock_ex() {
    ::InitializeCriticalSection(&m_csLock);
}

utils_thread_lock_ex::~utils_thread_lock_ex() {
    ::DeleteCriticalSection(&m_csLock);
}

CRITICAL_SECTION *utils_thread_lock_ex::get() {
    return &m_csLock;
}

void utils_thread_lock_ex::enter() {
    ::EnterCriticalSection(&m_csLock);
}

void utils_thread_lock_ex::leave() {
    ::LeaveCriticalSection(&m_csLock);
}
