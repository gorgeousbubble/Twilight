/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     utils_thread_lock.cpp
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-23
*/

#include "utils_thread_lock.h"

utils_thread_lock::utils_thread_lock(const CRITICAL_SECTION *lock, const bool safe) {
    m_pLock = const_cast<CRITICAL_SECTION*>(lock);
    m_bSafe = safe;

    if (m_bSafe) {
        ::EnterCriticalSection(m_pLock);
    }
}

utils_thread_lock::~utils_thread_lock() {
    if (m_bSafe) {
        ::LeaveCriticalSection(m_pLock);
    }
}
