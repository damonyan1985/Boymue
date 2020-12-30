package com.boymue.app.core.port;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.PixelFormat;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.boymue.app.core.port.init.BoymueInit;

public class SkiaPort {
    /** Called when the activity is first created. */
    public static void init(Activity activity) {
        // Makes and sets a SkiaDrawView as the only thing seen in this activity


        try {
            //BoymueInit.initFont(activity);
            // Load Skia and then the app shared object in this order
            System.loadLibrary("boymue");
        } catch (UnsatisfiedLinkError e) {
            Log.d("HelloSkia", "Link Error: " + e);
            return;
        }

        final SkiaDrawView fMainView = new SkiaDrawView(activity);
        activity.setContentView(fMainView);

        // Set a timer that will periodically request an update of the SkiaDrawView
//        Timer fAnimationTimer = new Timer();
//        fAnimationTimer.schedule(new TimerTask() {
//            public void run()
//            {
//                // This will request an update of the SkiaDrawView, even from other threads
//                fMainView.postInvalidate();
//            }
//        }, 0, 5); // 0 means no delay before the timer starts; 5 means repeat every 5 milliseconds
    }

    private static class SkiaDrawView extends SurfaceView implements SurfaceHolder.Callback {
        Bitmap fSkiaBitmap;
        public SkiaDrawView(Context ctx) {
            super(ctx);
            getHolder().addCallback(this);
            getHolder().setFormat(PixelFormat.TRANSLUCENT);
            setZOrderOnTop(true);
        }

        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            new Thread(() -> {
                // Create a bitmap for skia to draw into
//                fSkiaBitmap = Bitmap.createBitmap(
//                        holder.getSurfaceFrame().width(),
//                        holder.getSurfaceFrame().height(),
//                        Bitmap.Config.ARGB_8888);
//
//                // Call into our C++ code that renders to the bitmap using Skia
//                drawIntoBitmap(fSkiaBitmap, SystemClock.elapsedRealtime());
//
//                Canvas canvas = holder.lockHardwareCanvas();
//                // Present the bitmap on the screen
//                canvas.drawBitmap(fSkiaBitmap, 0, 0, null);
//
//                holder.unlockCanvasAndPost(canvas);

                initSurface(holder.getSurface(),
                        holder.getSurfaceFrame().width(),
                        holder.getSurfaceFrame().height());

                try {
                    Thread.sleep(1000 * 10);
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }).start();
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width,
                                   int height) {
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
        }

        @Override
        protected void onSizeChanged(int w, int h, int oldw, int oldh)
        {

        }

        @Override
        protected void onDraw(Canvas canvas) {

        }
    }


    private static native void drawIntoBitmap(Bitmap image, long elapsedTime);


    private static native void initSurface(Surface surface, int width, int height);
}
