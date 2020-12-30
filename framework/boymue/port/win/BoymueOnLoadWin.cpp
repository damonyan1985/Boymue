#include "BoymueOnLoadWin.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkString.h"
#include "SkSurface.h"
#include "Thread.h"
#include <stdio.h>
#include <string>

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.05
class UIThread : public boymue::Thread {
public:
    UIThread(HWND hwnd)
        : m_hwnd(hwnd)
    {
    }

    HBITMAP createGDIBitmap(int width, int height, void** ppBits)
    {
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = 0;

        HDC hdc = GetDC(NULL);
        HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, ppBits, 0, 0);
        ReleaseDC(NULL, hdc);
        return hBmp;
    }

    virtual void run()
    {
        /*
        LPVOID pBits = NULL;
        m_bitmap = createGDIBitmap(360, 640, &pBits);
        m_skbitmap = new SkBitmap();
        
        m_skbitmap->setInfo(SkImageInfo::Make(360, 640, kN32_SkColorType, kPremul_SkAlphaType));
        m_skbitmap->setPixels(pBits);
        printf("just test");

        SkCanvas* canvas = new SkCanvas(*m_skbitmap);
        SkPaint paint;
        paint.setXfermodeMode(SkXfermode::kClear_Mode);
        paint.setColor(SK_ColorRED);
        paint.setTextSize(SkIntToScalar(30));
        paint.setAntiAlias(true);

        // Draw some text
        SkString text("Skia is Best!");
        SkScalar fontHeight = paint.getFontSpacing();
        canvas->drawText(text.c_str(), text.size(),
            10, fontHeight,
            paint);

        

        HDC hdc = GetDC(m_hwnd);

        HDC comDC = ::CreateCompatibleDC(hdc);
        SelectObject(comDC, m_bitmap);
        BitBlt(comDC, 0, 0, 360, 640, hdc, 0, 0, SRCCOPY);

        ReleaseDC(m_hwnd, hdc);
        DeleteDC(comDC);*/
        HDC hdc = GetDC(m_hwnd);
        RECT rt;
        GetClientRect(m_hwnd, &rt);
        int bmpw = rt.right - rt.left;
        int bmph = rt.bottom - rt.top;

        const size_t bmpSize = sizeof(BITMAPINFOHEADER) + bmpw * bmph * sizeof(uint32_t);
        BITMAPINFO* bmpInfo = (BITMAPINFO*)new BYTE[bmpSize]();
        bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo->bmiHeader.biWidth = bmpw;

        bmpInfo->bmiHeader.biHeight = -bmph;

        bmpInfo->bmiHeader.biPlanes = 1;
        bmpInfo->bmiHeader.biBitCount = 32;
        bmpInfo->bmiHeader.biCompression = BI_RGB;
        void* pixels = bmpInfo->bmiColors;

        SkImageInfo info = SkImageInfo::Make(bmpw, bmph,
            kBGRA_8888_SkColorType, kPremul_SkAlphaType);

        SkSurface* surface = SkSurface::NewRasterDirect(info, pixels, bmpw * sizeof(uint32_t));
        SkCanvas* canvas = surface->getCanvas();

        canvas->clear(SK_ColorWHITE);

        Draw(canvas, bmpw, bmph);

        StretchDIBits(hdc, 0, 0, bmpw, bmph,
            0, 0, bmpw, bmph,
            pixels, bmpInfo,
            DIB_RGB_COLORS, SRCCOPY);

        delete[] bmpInfo;
        delete this;
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

        //canvas->save();
        SkPaint paint;
        paint.setStrokeWidth(1);
        paint.setARGB(0xff, 0xff, 0, 0);
        canvas->drawRect(SkRect::MakeXYWH(10, 10, 100, 100), paint);
        //canvas->restore();

        //canvas->save();
        SkPaint textpaint;
        //textpaint.reset();
        textpaint.setTextSize(16);
        textpaint.setARGB(0xff, 0xff, 0, 0);
        textpaint.setAntiAlias(true);
        //SkString string("Hello World");
        const wchar_t* text = L"Hello World";
        std::string str;
        WcharToChar(text, str, CP_UTF8);
        canvas->drawText(str.c_str(), str.size(), 20, 100, textpaint);
        //canvas->restore();
        canvas->flush();
    }

private:
    HWND m_hwnd;
    HBITMAP m_bitmap;
    SkBitmap* m_skbitmap;
};

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height)
{
    UIThread* thread = new UIThread(hwnd);
    thread->start();
}
