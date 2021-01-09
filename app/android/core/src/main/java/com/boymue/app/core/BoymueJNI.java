package com.boymue.app.core;

import android.view.Surface;

public class BoymueJNI {
  public static native void initSurface(Surface surface, int width, int height);
}