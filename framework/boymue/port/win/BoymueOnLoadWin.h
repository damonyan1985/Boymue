/*
 * Copyright Boymue Authors.
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
  void initWindow(HWND hwnd, int width, int height);
  void repaint();

private:
  void* m_app;
};

#endif  // !BoymueOnLoadWin_h
