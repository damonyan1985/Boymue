#include "BoymueOnLoadWin.h"
#include "Thread.h"
#include "SkBitmap.h"
#include "SkString.h"
#include "SkCanvas.h"
#include "SkSurface.h"
#include <stdio.h>

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

    void Draw(SkCanvas* canvas, int w, int h) {

        SkPaint paint;
        paint.setStrokeWidth(1);
        paint.setARGB(0xff, 0xff, 0, 0);

        for (int i = 0; i < h; i++)
        {
            canvas->drawPoint(0, i, paint);
        }

        for (int i = w; i > 0; i--)
        {
            canvas->drawPoint(i - 1, h - 1, paint);
        }

        SkPaint textpaint;
        textpaint.reset();
        textpaint.setTextSize(16);
        textpaint.setColor(SkColor(0xffff0000));
        textpaint.setAntiAlias(true);
        SkString string("Hello World");

        canvas->drawText(&string, string.size(), 1, 16, textpaint);

        canvas->flush();
    }

private:
    HWND m_hwnd;
    HBITMAP m_bitmap;
    SkBitmap* m_skbitmap;
};

void BoymueOnLoadWin::initWindow(HWND hwnd, int width, int height) {
    UIThread* thread = new UIThread(hwnd);
    thread->start();
}
