#include "breGfx.h"

#include <android/native_window.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <assert.h>

ANativeWindow *gNativeWindow;
static EGLDisplay gEglDpy;
static EGLSurface gEglSurface;
static EGLContext gEglCx;

void breGfxInit(ANativeWindow *win) {
    ANativeWindow_acquire(win);
    gNativeWindow = win;

    gEglDpy = eglGetDisplay(NULL);
    assert(gEglDpy != EGL_NO_DISPLAY);
    assert(eglInitialize(gEglDpy, NULL, NULL));

    // gladLoadEGLLoader((GLADloadproc)eglGetProcAddress);

    assert(eglBindAPI(EGL_OPENGL_ES_API));

    EGLint attributes[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_NONE
    };

    EGLConfig eConfig;
    EGLint numConfig;
    assert(eglChooseConfig(gEglDpy, attributes, &eConfig, 1, &numConfig));
    assert(numConfig > 0);

    gEglSurface = eglCreateWindowSurface(gEglDpy, eConfig, gNativeWindow, NULL);
    assert(gEglSurface);

    EGLint cxAttribs[] = {
            0x3098, 1,
            EGL_NONE
    };

    gEglCx = eglCreateContext(gEglDpy, eConfig, EGL_NO_CONTEXT, cxAttribs);
    assert(gEglCx);

    // eglMakeCurrent(gEglDpy, gEglCx)
    breGfxAcqCx();
    // gladLoadGLES1Loader((GLADloadproc) eglGetProcAddress);
}

void breGfxAcqCx() {
    assert(eglMakeCurrent(gEglDpy, gEglSurface, gEglSurface, gEglCx));
}

void breGfxRelCx() {
    eglMakeCurrent(NULL, NULL, NULL, NULL);
}

void breGfxSwap() {
    eglSwapBuffers(gEglDpy, gEglSurface);
}

void breGfxDestroy() {
    breGfxRelCx();

    ANativeWindow_release(gNativeWindow);
    gNativeWindow = NULL;
}
