#include <android/bitmap.h>
#include <jni.h>
#include <math.h>

#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkString.h"
#include "SkSurface.h"
#include "SkTime.h"

#include "AndroidContextGL.h"

#include "skia/include/core/SkColorFilter.h"
#include "skia/include/core/SkRect.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/GrContext.h"
#include "skia/include/gpu/gl/GrGLInterface.h"

#include <GLES2/gl2.h>
#include <android/native_window_jni.h>

using namespace boymue;
AndroidContextGL* context = nullptr;
AndroidEGLSurface* egl_surface = nullptr;

extern "C" JNIEXPORT void JNICALL Java_com_boymue_app_core_port_SkiaPort_initSurface(JNIEnv* env,
    jobject thiz, jobject jsurface, jint width, jint height)
{
    auto window = ANativeWindow_fromSurface(env, jsurface);
    context = new AndroidContextGL(new AndroidEnvironmentGL());

    egl_surface = context->CreateOnscreenSurface(new AndroidNativeWindow(window));
    egl_surface->MakeCurrent();

    glViewport(0, 0, width, height);

    glClearColor(1.0f, 0, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const GrGLInterface* fCurIntf = GrGLCreateNativeInterface();
    GrContext* context_ = GrContext::Create(kOpenGL_GrBackend, reinterpret_cast<GrBackendContext>(fCurIntf));

    //context_->setResourceCacheLimits(kGrCacheMaxCount, kGrCacheMaxByteSize);

    GrBackendRenderTargetDesc desc;
    desc.fWidth = width;
    desc.fHeight = height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 1;
    desc.fStencilBits = 8;

    GLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    desc.fRenderTargetHandle = buffer;

    auto render_target_ = context_->textureProvider()->wrapBackendRenderTarget(desc);

    SkSurfaceProps surface_props(
        SkSurfaceProps::InitType::kLegacyFontHost_InitType);

    SkSurface* surface = SkSurface::NewRenderTargetDirect(render_target_, &surface_props);
    SkCanvas* canvas = surface->getCanvas();

    canvas->drawColor(SK_ColorWHITE);

    // Setup a SkPaint for drawing our text
    SkPaint paint;
    paint.setColor(SK_ColorBLACK); // This is a solid black color for our text
    paint.setTextSize(SkIntToScalar(30)); // Sets the text size to 30 pixels
    paint.setAntiAlias(true); // We turn on anti-aliasing so that the text to looks good.

    // Draw some text
    SkString text("Skia is Best!");
    SkScalar fontHeight = paint.getFontSpacing();
    canvas->drawText(text.c_str(), text.size(), // text's data and length
        10, fontHeight, // X and Y coordinates to place the text
        paint); // SkPaint to tell how to draw the text

    // Adapt the SkPaint for drawing blue lines
    paint.setAntiAlias(false); // Turning off anti-aliasing speeds up the line drawing
    paint.setColor(0xFF0000FF); // This is a solid blue color for our lines
    paint.setStrokeWidth(SkIntToScalar(2)); // This makes the lines have a thickness of 2 pixels

    context_->flush();

    egl_surface->SwapBuffers();
}

/**
 * Draws something into the given bitmap
 * @param  env
 * @param  thiz
 * @param  dstBitmap   The bitmap to place the results of skia into
 * @param  elapsedTime The number of milliseconds since the app was started
 */
extern "C" JNIEXPORT void JNICALL Java_com_boymue_app_core_port_SkiaPort_drawIntoBitmap(JNIEnv* env,
    jobject thiz, jobject dstBitmap, jlong elapsedTime)
{
    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;
    AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);

    SkImageInfo info = SkImageInfo::MakeN32Premul(dstInfo.width, dstInfo.height);

    // Create a surface from the given bitmap
    SkAutoTUnref<SkSurface> surface(SkSurface::NewRasterDirect(info, dstPixels, dstInfo.stride));
    SkCanvas* canvas = surface->getCanvas();

    // Draw something "interesting"

    // Clear the canvas with a white color
    canvas->drawColor(SK_ColorWHITE);

    // Setup a SkPaint for drawing our text
    SkPaint paint;
    paint.setColor(SK_ColorBLACK); // This is a solid black color for our text
    paint.setTextSize(SkIntToScalar(30)); // Sets the text size to 30 pixels
    paint.setAntiAlias(true); // We turn on anti-aliasing so that the text to looks good.

    // Draw some text
    SkString text("Skia is Best!");
    SkScalar fontHeight = paint.getFontSpacing();
    canvas->drawText(text.c_str(), text.size(), // text's data and length
        10, fontHeight, // X and Y coordinates to place the text
        paint); // SkPaint to tell how to draw the text

    // Adapt the SkPaint for drawing blue lines
    paint.setAntiAlias(false); // Turning off anti-aliasing speeds up the line drawing
    paint.setColor(0xFF0000FF); // This is a solid blue color for our lines
    paint.setStrokeWidth(SkIntToScalar(2)); // This makes the lines have a thickness of 2 pixels

    // Draw some interesting lines using trig functions
    for (int i = 0; i < 100; i++) {
        float x = (float)i / 99.0f;
        float offset = elapsedTime / 1000.0f;
        canvas->drawLine(sin(x * M_PI + offset) * 800.0f, 0, // first endpoint
            cos(x * M_PI + offset) * 800.0f, 800, // second endpoint
            paint); // SkPapint to tell how to draw the line
    }

    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, dstBitmap);
}
