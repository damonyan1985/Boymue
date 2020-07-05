#ifndef PlatformViewWin_h
#define PlatformViewWin_h

#include "PlatformView.h"
#include <Windows.h>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
namespace boymue {
class PlatformViewWin : public PlatformView {

private:
    HWND m_wnd;
};
}
#endif // !PlatformViewWin_h

