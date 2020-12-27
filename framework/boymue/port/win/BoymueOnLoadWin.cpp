#include "BoymueOnLoadWin.h"
#include "Thread.h"
#include "SkBitmap.h"
#include "SkString.h"
#include "SkCanvas.h"
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
        LPVOID pBits = NULL;
        m_bitmap = createGDIBitmap(360, 640, &pBits);
        m_skbitmap = new SkBitmap();
        
        m_skbitmap->setInfo(SkImageInfo::Make(360, 640, kN32_SkColorType, kPremul_SkAlphaType));
        m_skbitmap->setPixels(pBits);
        printf("just test");

        SkCanvas* canvas = new SkCanvas(*m_skbitmap);
        SkPaint paint;
        paint.setColor(SK_ColorRED);
        paint.setTextSize(SkIntToScalar(30));
        paint.setAntiAlias(true);

        // Draw some text
        SkString text("Skia is Best!");
        SkScalar fontHeight = paint.getFontSpacing();
        canvas->drawText(text.c_str(), text.size(),
            10, fontHeight,
            paint);

        delete this;
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
