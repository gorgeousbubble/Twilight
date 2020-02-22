/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     logs_glog.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-22
*/

#ifndef TWILIGHT_LOGS_GLOG_H
#define TWILIGHT_LOGS_GLOG_H

#include <iostream>
#include "logging.h"

#pragma comment(lib, "glog.lib")

#ifdef TWILIGHT_EXPORTS
#define TWILIGHT_LOGS_GLOG_API __declspec(dllexport)
#else
#define TWILIGHT_LOGS_GLOG_API __declspec(dllimport)
#endif

#define TWILIGHT_LOGS_GLOG_CALLMETHOD __stdcall

class TWILIGHT_LOGS_GLOG_API logs_glog {

};


#endif //TWILIGHT_LOGS_GLOG_H
