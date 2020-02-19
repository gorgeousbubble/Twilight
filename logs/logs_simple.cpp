/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file		logs_simple.cpp
* @brief	The Twilight Project.
* @author	alopex
* @version	v1.00a
* @date		2020-02-18
*/

#include "logs_simple.h"

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

void logs_simple::write(char *str, ...) {

}
