package io.github.usernameak.brewemulator;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.PixelFormat;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class EmulatorSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private final EmulatorMainContext emulatorMainContext;

    public EmulatorSurfaceView(Context context, EmulatorMainContext emulatorMainContext) {
        super(context);

        this.emulatorMainContext = emulatorMainContext;

        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        System.out.println("surfaceCreated");
        emulatorMainContext.surfaceHolder = holder;
        emulatorMainContext.surface = holder.getSurface();

        emulatorMainContext.checkInitialized();
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }
}
