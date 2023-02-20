// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2020.07.14
#include <android/native_window_jni.h>
#include <jni.h>

#include "PaintContextAndroid.h"
#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkString.h"
#include "SkSurface.h"
#include "SkTime.h"
#include "SkTypeface.h"
#include "skia/include/core/SkColorFilter.h"
#include "skia/include/core/SkRect.h"
#include "skia/include/core/SkSurface.h"

const char* kBoymueJNIClassName = "com/boymue/app/core/BoymueJNI";

static void nativeInitSurface(JNIEnv* env, jobject thiz,
                                               jobject jsurface, jint width,
                                               jint height) {
  auto window = ANativeWindow_fromSurface(env, jsurface);
  boymue::PaintContextAndroid* painter = new boymue::PaintContextAndroid();
  painter->initContext(new boymue::AndroidNativeWindow(window), width, height);

  // JUST test
  SkCanvas* canvas = painter->canvas();
  canvas->drawColor(SK_ColorWHITE);

  SkTypeface* typeface = SkTypeface::CreateFromFile(
      "/system/fonts/NotoSansCJK-Regular.ttc", SkTypeface::kNormal);
  // Setup a SkPaint for drawing our text
  SkPaint paint;
  if (typeface) {
    paint.setTypeface(typeface);
  }
  paint.setColor(SK_ColorRED);  // This is a solid black color for our text
  paint.setTextSize(SkIntToScalar(30));  // Sets the text size to 30 pixels
  paint.setAntiAlias(
      true);  // We turn on anti-aliasing so that the text to looks good.

  // Draw some text
  SkString text("Skia纯测试");
  SkScalar fontHeight = paint.getFontSpacing();
  canvas->drawText(text.c_str(), text.size(),  // text's data and length
                   10, fontHeight,  // X and Y coordinates to place the text
                   paint);          // SkPaint to tell how to draw the text

  // Adapt the SkPaint for drawing blue lines
  paint.setAntiAlias(
      false);  // Turning off anti-aliasing speeds up the line drawing
  paint.setColor(0xFF0000FF);  // This is a solid blue color for our lines
  paint.setStrokeWidth(
      SkIntToScalar(2));  // This makes the lines have a thickness of 2 pixels

  painter->submit();
}

static jlong nativeCreateApplication(JNIEnv* env, jobject thiz,
                                               jobject jsurface, jint width,
                                               jint height) {
    return 0;                                            
}

static JNINativeMethod sBoymueAppMethods[] = {
    { "nativeInitSurface", "(Landroid/view/Surface;II)V", (void*)nativeInitSurface },
    { "nativeCreateApplication", "(Landroid/view/Surface;II)J", (void*)nativeCreateApplication },
};

extern int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* methods, int numMethods);

/*
* Register native methods for all classes we know about.
*/
int registerBoymueAppNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env, kBoymueJNIClassName, sBoymueAppMethods,
            sizeof(sBoymueAppMethods) / sizeof(sBoymueAppMethods[0]))) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}