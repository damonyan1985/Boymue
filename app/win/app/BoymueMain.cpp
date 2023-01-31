#include <Windows.h>
#include "BoymueOnLoadWin.h"
#include "BoyiaConsole.h"

LRESULT CALLBACK BoymueWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow
)
{
    yanbo::BoyiaConsole console;
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
        BoymueOnLoadWin::initWindow(hWnd, 
            rect.right - rect.left,
            rect.bottom - rect.top);
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        BoymueOnLoadWin::repaint();
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