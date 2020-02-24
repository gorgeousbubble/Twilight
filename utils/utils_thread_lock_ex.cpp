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

//----------------------------------------------
// @Function: utils_thread_lock_ex
// @Purpose: construct function
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
utils_thread_lock_ex::utils_thread_lock_ex() {
    ::InitializeCriticalSection(&m_csLock);
}

//----------------------------------------------
// @Function: ~utils_thread_lock_ex
// @Purpose: destruct function
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
utils_thread_lock_ex::~utils_thread_lock_ex() {
    ::DeleteCriticalSection(&m_csLock);
}

//----------------------------------------------
// @Function: get
// @Purpose: get the critical section pointer
// @Since: v1.00a
// @Para: None
// @Return: m_csLock's pointer
//----------------------------------------------
CRITICAL_SECTION *utils_thread_lock_ex::get() {
    return &m_csLock;
}

//----------------------------------------------
// @Function: enter
// @Purpose: enter critical section
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void utils_thread_lock_ex::enter() {
    ::EnterCriticalSection(&m_csLock);
}

//----------------------------------------------
// @Function: leave
// @Purpose: leave critical section
// @Since: v1.00a
// @Para: None
// @Return: None
//----------------------------------------------
void utils_thread_lock_ex::leave() {
    ::LeaveCriticalSection(&m_csLock);
}
