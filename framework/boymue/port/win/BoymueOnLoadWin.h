/*
* Author damonyan
* All Copyright reserved
*/

#ifndef BoymueOnLoadWin_h
#define BoymueOnLoadWin_h

#include <Windows.h>

#ifdef _WINDLL
#define BOYMUE_PORTING_API __declspec(dllexport)
#else
#define BOYMUE_PORTING_API __declspec(dllimport)
#endif

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
class BOYMUE_PORTING_API BoymueOnLoadWin {
public:
    static void initWindow(HWND hwnd, int width, int height);
    static void repaint();
};

#endif // !BoymueOnLoadWin_h
