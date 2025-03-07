#include "BoymueOnLoadWin.h"
#include "BoyiaConsole.h"
#include <stdio.h>

BoymueOnLoadWin sBoymue;

LRESULT CALLBACK BoymueWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow
)
{
    yanbo::BoyiaConsole console;
    printf("BoymueWndProc start\n");
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
    
    WNDCLASS wndClass;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = BoymueWndProc;
    wndClass.lpszClassName = L"Boymue";
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_VREDRAW | CS_HREDRAW;

    ::RegisterClass(&wndClass);
    HWND hWnd = ::CreateWindow(L"Boymue", L"BoymueWindow", dwStyle, 0, 0, 360, 640, nullptr, nullptr, hInstance, nullptr);
    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK BoymueWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE: {
        RECT rect;
        ::GetClientRect(hWnd, &rect);
        sBoymue.initWindow(hWnd,
            rect.right - rect.left,
            rect.bottom - rect.top);
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        sBoymue.repaint();
        EndPaint(hWnd, &ps);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

#ifdef _WIN64
#pragma comment(linker, "/INCLUDE:_tls_used")
#else
#pragma comment(linker, "/INCLUDE:__tls_used")
#endif // _WIN64

void print_console(const char* szMsg) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(out, szMsg, strlen(szMsg), NULL, NULL);
}

void NTAPI TLS_CALLBACK(PVOID handle, DWORD reason, PVOID reserved) {
    char szMsg[80] = { 0 };
    wsprintfA(szMsg, "TLS_CALLBACK() handle=%x, reason=%d\n", handle, reason);
    print_console(szMsg);
}

extern "C"
#ifdef _WIN64
#pragma const_seg(".CRT$XLX")
const
#else
#pragma data_seg(".CRT$XLX")
#endif

PIMAGE_TLS_CALLBACK pTLS_CALLBACKs[] = { TLS_CALLBACK, 0 };

#pragma data_seg()