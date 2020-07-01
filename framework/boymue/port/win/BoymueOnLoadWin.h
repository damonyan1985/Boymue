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

class BOYMUE_PORTING_API BoymueOnLoadWin {
public:
    static void init();
};

#endif // !BoymueOnLoadWin_h
