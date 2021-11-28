package io.github.usernameak.brewemulator;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.ContextThemeWrapper;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TableLayout;
import android.widget.TableRow;

public class EmulatorKeypad {
    public static final String[][] KEYPAD_LABELS = {
            {"1", "2", "3", "-", "↑", "-"},
            {"4", "5", "6", "←", "•", "→"},
            {"7", "8", "9", "+", "↓", null},
            {"*", "0", "#", "S", "C", "E"},
    };

    public static final int[][] KEYPAD_KEYCODES = {
            {0xE022, 0xE023, 0xE024, 0xE036, 0xE031, 0xE037},
            {0xE025, 0xE026, 0xE027, 0xE033, 0xE035, 0xE034},
            {0xE028, 0xE029, 0xE02A, 0xE083, 0xE032, 0xE010},
            {0xE02B, 0xE021, 0xE02C, 0xE02F, 0xE030, 0xE02E},
    };

    public interface IKeypadHandler {
        void onButtonDown(int code);
        void onButtonUp(int code);
    }

    @SuppressLint("ClickableViewAccessibility")
    public static ViewGroup buildKeypad(Context ctx, final IKeypadHandler handler) {
        TableLayout tl = new TableLayout(ctx);
        tl.setStretchAllColumns(true);
        for(int row = 0; row < KEYPAD_LABELS.length; row++) {
            TableRow tr = new TableRow(ctx);
            for(int column = 0; column < KEYPAD_LABELS[row].length; column++) {
                String label = KEYPAD_LABELS[row][column];
                if(label != null) {
                    Button btn = new Button(new ContextThemeWrapper(ctx,R.style.KeypadButton), null, 0);
                    btn.setLayoutParams(new TableRow.LayoutParams(column));
                    btn.setText(KEYPAD_LABELS[row][column]);
                    final int code = KEYPAD_KEYCODES[row][column];
                    btn.setOnTouchListener((v, event) -> {
                        if(event.getAction() == MotionEvent.ACTION_DOWN) {
                            handler.onButtonDown(code);
                        } else if(event.getAction() == MotionEvent.ACTION_CANCEL) {
                            handler.onButtonUp(code);
                        } else if(event.getAction() == MotionEvent.ACTION_UP) {
                            handler.onButtonUp(code);
                        }
                        return false;
                    });
                    tr.addView(btn);
                }
            }
            tl.addView(tr);
        }

        return tl;
    }
}
