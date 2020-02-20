/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file		logs_simple.h
* @brief	The Twilight Project.
* @author	alopex
* @version	v1.00a
* @date		2020-02-18
*/

#ifndef TWILIGHT_LOGS_SIMPLE_H
#define TWILIGHT_LOGS_SIMPLE_H

#include <Windows.h>
#include <Shlwapi.h>

#include <iostream>
#include <fstream>

#pragma comment(lib, "Shlwapi.lib")

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_LOGS_SIMPLE_API __declspec(dllexport)
#else
#define TWILIGHT_LOGS_SIMPLE_API __declspec(dllimport)
#endif

#define TWILIGHT_LOGS_SIMPLE_CALLMETHOD __stdcall

class TWILIGHT_LOGS_SIMPLE_API logs_simple {
private:
    static void get_path(char* str, int size);
    static void create();
    static void write(LPCSTR str, ...);
public:
    static void debug(LPCSTR str, ...);
    static void info(LPCSTR str, ...);
    static void event(LPCSTR str, ...);
    static void warning(LPCSTR str, ...);
    static void error(LPCSTR str, ...);
    static void critical(LPCSTR str, ...);
};


#endif //TWILIGHT_LOGS_SIMPLE_H
