package io.github.usernameak.brewemulator;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("brewemu");
    }

    private ViewGroup keypad;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        FrameLayout fr = new FrameLayout(this);

        EmulatorSurfaceView surfaceView = new EmulatorSurfaceView(getApplicationContext());
        surfaceView.setLayoutParams(new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        fr.addView(surfaceView);

        keypad = EmulatorKeypad.buildKeypad(this, new EmulatorKeypad.IKeypadHandler() {
            @Override
            public void onButtonDown(int code) {
                brewEmuKeyDown(code);
            }

            @Override
            public void onButtonUp(int code) {
                brewEmuKeyUp(code);
            }
        });
        keypad.setLayoutParams(new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, Gravity.BOTTOM));
        fr.addView(keypad);

        fr.setFocusable(false);

        setContentView(fr);

        brewEmuJNIStartup();
    }

    private void hideKeypad() {
        keypad.setVisibility(View.GONE);
    }

    private int translateKeycode(int keyCode) {
        int avk = 0xE010;
        switch(keyCode) {
            case KeyEvent.KEYCODE_DPAD_DOWN: avk = 0xE032; break;
            case KeyEvent.KEYCODE_DPAD_UP: avk = 0xE031; break;
            case KeyEvent.KEYCODE_DPAD_LEFT: avk = 0xE033; break;
            case KeyEvent.KEYCODE_DPAD_RIGHT: avk = 0xE034; break;

            case KeyEvent.KEYCODE_DPAD_CENTER:
            case KeyEvent.KEYCODE_ENTER:
                avk = 0xE035; break;

            case KeyEvent.KEYCODE_CLEAR:
            case KeyEvent.KEYCODE_ESCAPE:
            case KeyEvent.KEYCODE_C:
            case KeyEvent.KEYCODE_DEL:
                avk = 0xE030; break;

            case KeyEvent.KEYCODE_MENU:
            case KeyEvent.KEYCODE_W:
                avk = 0xE036; break;

            case KeyEvent.KEYCODE_BACK:
            case KeyEvent.KEYCODE_E:
                avk = 0xE037; break;

            case KeyEvent.KEYCODE_X:
            case KeyEvent.KEYCODE_ENDCALL:
                avk = 0xE02E; break;

            case KeyEvent.KEYCODE_Z:
            case KeyEvent.KEYCODE_CALL:
                avk = 0xE02F; break;

            case KeyEvent.KEYCODE_0:
            case KeyEvent.KEYCODE_NUMPAD_0:
                avk = 0xE021; break;

            case KeyEvent.KEYCODE_1:
            case KeyEvent.KEYCODE_NUMPAD_1:
                avk = 0xE022; break;

            case KeyEvent.KEYCODE_2:
            case KeyEvent.KEYCODE_NUMPAD_2:
                avk = 0xE023; break;

            case KeyEvent.KEYCODE_3:
            case KeyEvent.KEYCODE_NUMPAD_3:
                avk = 0xE024; break;

            case KeyEvent.KEYCODE_4:
            case KeyEvent.KEYCODE_NUMPAD_4:
                avk = 0xE025; break;

            case KeyEvent.KEYCODE_5:
            case KeyEvent.KEYCODE_NUMPAD_5:
                avk = 0xE026; break;

            case KeyEvent.KEYCODE_6:
            case KeyEvent.KEYCODE_NUMPAD_6:
                avk = 0xE027; break;

            case KeyEvent.KEYCODE_7:
            case KeyEvent.KEYCODE_NUMPAD_7:
                avk = 0xE028; break;

            case KeyEvent.KEYCODE_8:
            case KeyEvent.KEYCODE_NUMPAD_8:
                avk = 0xE029; break;

            case KeyEvent.KEYCODE_9:
            case KeyEvent.KEYCODE_NUMPAD_9:
                avk = 0xE02A; break;

            case KeyEvent.KEYCODE_STAR:
            case KeyEvent.KEYCODE_S:
            case KeyEvent.KEYCODE_NUMPAD_MULTIPLY:
                avk = 0xE02B; break;

            case KeyEvent.KEYCODE_POUND:
            case KeyEvent.KEYCODE_D:
            case KeyEvent.KEYCODE_NUMPAD_DIVIDE:
                avk = 0xE02C; break;
        }

        return avk;
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        int brewCode = translateKeycode(event.getKeyCode());
        if(brewCode != 0xE010) {
            if(event.getAction() == KeyEvent.ACTION_DOWN) {
                brewEmuKeyDown(brewCode);
                return true;
            } else if(event.getAction() == KeyEvent.ACTION_UP) {
                brewEmuKeyUp(brewCode);
                return true;
            }
        }
        return super.dispatchKeyEvent(event);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        brewEmuJNIShutdown();
    }

    public void setUseLandscapeOrientation(boolean value) {
        if(value) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        } else {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
        }
    }

    public native boolean brewEmuKeyUp(int keyCode);
    public native boolean brewEmuKeyDown(int keyCode);

    public native void brewEmuJNIStartup();
    public native void brewEmuJNIShutdown();
}