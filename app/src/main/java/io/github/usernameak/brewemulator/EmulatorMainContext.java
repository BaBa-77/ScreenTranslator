package io.github.usernameak.brewemulator;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class EmulatorMainContext {
    public Surface surface;
    public SurfaceHolder surfaceHolder;
    public MainActivity mainActivity;
    public boolean running = false;

    public void checkInitialized() {
        if(!running) {
            if(surface != null && mainActivity != null) {
                running = true;
                System.out.println("brewEmuJniStartup");

                mainActivity.brewEmuJNIStartup(surface);
            }
        }
    }
}
