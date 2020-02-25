/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2020, Gorgeous Bubble
*     All rights reserved.
*
* @proj     Twilight
* @file     utils_safe.h
* @brief    The Twilight Project.
* @author   alopex
* @version  v1.00a
* @date     2020-02-25
*/

#ifndef TWILIGHT_UTILS_SAFE_H
#define TWILIGHT_UTILS_SAFE_H

#include <iostream>

template <class T>
void safe_delete(T*& t) {
    if (nullptr != t) {
        delete t;
        t = nullptr;
    }
}

template <class T>
void safe_delete_array(T*& t) {
    if (nullptr != t) {
        delete[] t;
        t = nullptr;
    }
}

template <class T>
void safe_release(T*& t) {
    if (nullptr != t) {
        t->Release();
        t = nullptr;
    }
}

#endif //TWILIGHT_UTILS_SAFE_H
