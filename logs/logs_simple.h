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

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_LOGS_SIMPLE_API __declspec(dllexport)
#else
#define TWILIGHT_LOGS_SIMPLE_API __declspec(dllimport)
#endif

#define TWILIGHT_LOGS_SIMPLE_CALLMETHOD __stdcall

class logs_simple {
private:

};


#endif //TWILIGHT_LOGS_SIMPLE_H
