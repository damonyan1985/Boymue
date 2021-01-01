#include "BoymueOnLoadWin.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkString.h"
#include "SkSurface.h"
#include "Thread.h"
#include "TaskThread.h"
#include <stdio.h>
#include <string>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
class UIRuntime {
public:
    UIRuntime(HWND hwnd)
        : m_hwnd(hwnd)
        , m_bitmapInfo(nullptr)
    {
    }

    virtual void run()
    {
        HDC hdc = GetDC(m_hwnd);
        RECT rt;
        GetClientRect(m_hwnd, &rt);
        int bmpw = rt.right - rt.left;
        int bmph = rt.bottom - rt.top;

        const size_t bmpSize = sizeof(BITMAPINFOHEADER) + bmpw * bmph * sizeof(uint32_t);
        m_bitmapInfo = (BITMAPINFO*)new BYTE[bmpSize]();
        m_bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        m_bitmapInfo->bmiHeader.biWidth = bmpw;

        m_bitmapInfo->bmiHeader.biHeight = -bmph;

        m_bitmapInfo->bmiHeader.biPlanes = 1;
        m_bitmapInfo->bmiHeader.biBitCount = 32;
        m_bitmapInfo->bmiHeader.biCompression = BI_RGB;
        void* pixels = m_bitmapInfo->bmiColors;

        SkImageInfo info = SkImageInfo::Make(bmpw, bmph,
            kBGRA_8888_SkColorType, kPremul_SkAlphaType);

        SkSurface* surface = SkSurface::NewRasterDirect(info, pixels, bmpw * sizeof(uint32_t));
        SkCanvas* canvas = surface->getCanvas();

        canvas->clear(SK_ColorWHITE);

        Draw(canvas, bmpw, bmph);

        StretchDIBits(hdc, 0, 0, bmpw, bmph,
            0, 0, bmpw, bmph,
            pixels, m_bitmapInfo,
            DIB_RGB_COLORS, SRCCOPY);

        //delete[] m_bitmapInfo;
        //delete this;
    }

    void WcharToChar(const wchar_t* wp, std::string& text, size_t encode)
    {
        //std::string str;
        int len = WideCharToMultiByte(encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
        char* chs = new char[len + 1];
        WideCharToMultiByte(encode, 0, wp, wcslen(wp), chs, len, NULL, NULL);
        chs[len] = '\0';
        text = chs;
    }

    void Draw(SkCanvas* canvas, int w, int h)
    {
        
        SkPaint paint;
        paint.setStrokeWidth(1);
        paint.setARGB(0xff, 0xff, 0, 0);
        canvas->drawRect(SkRect::MakeXYWH(10, 10, 100, 100), paint);
        

        SkPaint textpaint;
        textpaint.setTextSize(16);
        textpaint.setColor(SK_ColorRED);
        textpaint.setAntiAlias(true);
        //SkString string("Hello World");
        const wchar_t* text = L"Hello World";
        std::string str;
        WcharToChar(text, str, CP_UTF8);
        canvas->drawText(str.c_str(), str.size(), 20, 300, textpaint);
        //canvas->restore();
        //canvas->flush();
    }

    BITMAPINFO* getBitmap() const
    {
        return m_bitmapInfo;
    }

    HWND getWindow() const 
    {
        return m_hwnd;
    }

private:
    HWND m_hwnd;
    SkBitmap* m_skbitmap;
    BITMAPINFO* m_bitmapInfo;
};

static UIRuntime* s_uiRuntime;
static boymue::TaskThread* getUiThread()
{
    static boymue::TaskThread uiThread;
    return &uiThread;
}

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height)
{
    s_uiRuntime = new UIRuntime(hwnd);
    //UIThread* thread = new UIThread(hwnd);
    //thread->start();
    getUiThread()->start();
    getUiThread()->getTaskRunner().postTask([&]() {
        s_uiRuntime->run();
    });
}

void BoymueOnLoadWin::repaint()
{
    getUiThread()->getTaskRunner().postTask([&]() {
        if (!s_uiRuntime->getWindow()) {
            return;
        }

        HWND hwnd = s_uiRuntime->getWindow();
        HDC hdc = GetDC(s_uiRuntime->getWindow());
        BITMAPINFO* bmp = s_uiRuntime->getBitmap();
        if (bmp) {
            RECT rt;
            GetClientRect(hwnd, &rt);
            int bmpw = rt.right - rt.left;
            int bmph = rt.bottom - rt.top;

            StretchDIBits(hdc, 0, 0, bmpw, bmph,
                0, 0, bmpw, bmph,
                bmp->bmiColors, bmp,
                DIB_RGB_COLORS, SRCCOPY);
        }
    });
}
