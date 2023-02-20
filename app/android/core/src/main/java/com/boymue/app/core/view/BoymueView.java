package com.boymue.app.core.view;

import android.content.Context;
import android.graphics.PixelFormat;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

import com.boymue.app.core.BoymueJNI;

import java.util.ArrayList;
import java.util.List;

public class BoymueView extends SurfaceView implements SurfaceHolder.Callback {

    private List<SurfaceLifeObserver> mObservers;

    static {
        System.loadLibrary("boymue");
    }

    public BoymueView(Context context) {
        super(context);
        initSurface();
    }

    private void initSurface() {
        setZOrderOnTop(true);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        mObservers = new ArrayList<>();
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        for (SurfaceLifeObserver observer : mObservers) {
            observer.surfaceCreated(holder);
        }

        new Thread(() -> {
            BoymueJNI.nativeInitSurface(holder.getSurface(),
                    holder.getSurfaceFrame().width(),
                    holder.getSurfaceFrame().height());
        }).start();
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        for (SurfaceLifeObserver observer : mObservers) {
            observer.surfaceChanged(holder, format, width, height);
        }
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        for (SurfaceLifeObserver observer : mObservers) {
            observer.surfaceDestroyed(holder);
        }
    }

    public interface SurfaceLifeObserver {
        void surfaceCreated(SurfaceHolder holder);

        void surfaceChanged(SurfaceHolder holder, int format,
                            int width, int height);

        void surfaceDestroyed(SurfaceHolder holder);
    }
}