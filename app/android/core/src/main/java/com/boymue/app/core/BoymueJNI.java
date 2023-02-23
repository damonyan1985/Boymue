package com.boymue.app.core;

import android.view.Surface;

/**
 * boymue app应用底层JNI接口
 */
public class BoymueJNI {
  public static native void nativeInitSurface(Surface surface, int width, int height);

  /**
   * 创建一个Boymue应用对象，返回对象指针
   * @param surface
   * @param width
   * @param height
   * @return
   */
  public static native long nativeCreateApplication(Surface surface, int width, int height);
}