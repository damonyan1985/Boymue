package com.boymue.app.core.port;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;

public class SkiaPort {
    /** Called when the activity is first created. */
    public static void init(Activity activity) {
        // Makes and sets a SkiaDrawView as the only thing seen in this activity


        try {
            // Load Skia and then the app shared object in this order
            System.loadLibrary("boymue");
        } catch (UnsatisfiedLinkError e) {
            Log.d("HelloSkia", "Link Error: " + e);
            return;
        }

        final SkiaDrawView fMainView = new SkiaDrawView(activity);
        activity.setContentView(fMainView);

        // Set a timer that will periodically request an update of the SkiaDrawView
        Timer fAnimationTimer = new Timer();
        fAnimationTimer.schedule(new TimerTask() {
            public void run()
            {
                // This will request an update of the SkiaDrawView, even from other threads
                fMainView.postInvalidate();
            }
        }, 0, 5); // 0 means no delay before the timer starts; 5 means repeat every 5 milliseconds
    }

    private static class SkiaDrawView extends View {
        Bitmap fSkiaBitmap;
        public SkiaDrawView(Context ctx) {
            super(ctx);
        }

        @Override
        protected void onSizeChanged(int w, int h, int oldw, int oldh)
        {
            // Create a bitmap for skia to draw into
            fSkiaBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        }

        @Override
        protected void onDraw(Canvas canvas) {
            // Call into our C++ code that renders to the bitmap using Skia
            drawIntoBitmap(fSkiaBitmap, SystemClock.elapsedRealtime());

            // Present the bitmap on the screen
            canvas.drawBitmap(fSkiaBitmap, 0, 0, null);
        }
    }


    private static native void drawIntoBitmap(Bitmap image, long elapsedTime);
}
