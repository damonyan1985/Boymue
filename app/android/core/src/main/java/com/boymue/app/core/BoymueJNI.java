package com.boymue.app.core;

import android.view.Surface;

public class BoymueJNI {
  public static native void nativeInitSurface(Surface surface, int width, int height);
  public static native long nativeCreateApplication(Surface surface, int width, int height);
}