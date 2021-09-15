package io.github.usernameak.brewemulator;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.PixelFormat;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class EmulatorSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private final EmulatorMainContext emulatorMainContext;

    public EmulatorSurfaceView(Context context, EmulatorMainContext emulatorMainContext) {
        super(context);

        this.emulatorMainContext = emulatorMainContext;

        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        System.out.println("surfaceCreated");
        emulatorMainContext.surfaceHolder = holder;
        emulatorMainContext.surface = holder.getSurface();

        emulatorMainContext.checkInitialized();
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
    }
}
