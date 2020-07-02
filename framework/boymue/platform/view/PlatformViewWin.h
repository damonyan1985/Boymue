#ifndef PlatformViewWin_h
#define PlatformViewWin_h

#include "PlatformView.h"
#include <Windows.h>
namespace boymue {
class PlatformViewWin : public PlatformView {

private:
    HWND m_wnd;
};
}
#endif // !PlatformViewWin_h

