#include "brewemu.h"

extern "C" {
#include <AEE_OEMDispatch.h>
#include <OEMNotify.h>
#include <AEEConfig.h>
#include <AEEModTable.h>
#include <BREWVersion.h>
#undef EALREADY
}

#include "bre2/breConfig.h"
#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <android/keycodes.h>
#include <thread>
#include <AEE_OEM.h>

const char *gExternalFilesDir = nullptr;

static void mkdirs(const char *dir) {
    char tmp[PATH_MAX];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
}

static void acquireExternalFilesDir(JNIEnv *env, jobject thiz) {
    jclass activity_clazz = env->FindClass("android/app/Activity");
    jclass file_clazz = env->FindClass("java/io/File");
    jmethodID getExternalFilesDir = env->GetMethodID(activity_clazz, "getExternalFilesDir",
                                                     "(Ljava/lang/String;)Ljava/io/File;");
    jmethodID getAbsolutePath = env->GetMethodID(file_clazz, "getAbsolutePath",
                                                 "()Ljava/lang/String;");
    auto externalFilesDir = env->CallObjectMethod(thiz, getExternalFilesDir,
                                                            (jobject) nullptr);

    auto absPath = (jstring) env->CallObjectMethod(externalFilesDir, getAbsolutePath);

    auto externalFilesDirStr = env->GetStringUTFChars(absPath, nullptr);
    gExternalFilesDir = strdup(externalFilesDirStr);

    mkdirs(gExternalFilesDir);
}

extern "C" void AEEREGISTRY_EnableDebugMsg(int enable);
extern "C" void AEENOTIFY_EnableDebugMsg(int enable);
extern "C" void AEEMIFPROP_EnableDebugMsg(int enable);
extern "C" void AEEMIF_EnableDebugMsg(int enable);
extern "C" void AEELICENSE_EnableDebugMsg(int enable);
extern "C" void AEEPRELOAD_EnableDebugMsg(int enable);
extern "C" void AEEFASTLOAD_EnableDebugMsg(int enable);
extern "C" void AEEMOD_EnableDebugMsg(int enable);
extern "C" void AEESERVICE_EnableDebugMsg(int enable);
extern "C" void AEEDISPATCH_EnableDebugMsg(int enable);
extern "C" void AEEALARMS_EnableDebugMsg(int enable);
extern "C" void AEEEVENT_EnableDebugMsg(int enable);
extern "C" void AEEPRIV_EnableDebugMsg(int enable);
extern "C" void AEEKEY_EnableDebugMsg(int enable);
extern "C" void AEEAPP_EnableDebugMsg(int enable);
extern "C" void AEEDBGKEY_EnableDebugMsg(int enable);
extern "C" void AEEFREEMEM_EnableDebugMsg(int enable);
extern "C" void AEESTACK_EnableDebugMsg(int enable);

static AEECallback gCBStartLauncherApp;

#include "bre2/breStartup.h"
#include <cassert>
#include <AEE_OEMEvent.h>
#include "bre2/breGfx.h"

static bool isBREWRunning = false;

extern "C"
JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuJNIStartup(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    breGfxInit(nativeWindow);
    ANativeWindow_release(nativeWindow);

    acquireExternalFilesDir(env, thiz);

#ifdef BRE_ENABLE_PLATFORM_DEBUG
    AEEREGISTRY_EnableDebugMsg(1);
    AEENOTIFY_EnableDebugMsg(1);
    AEEMIFPROP_EnableDebugMsg(1);
    AEEMIF_EnableDebugMsg(1);
    AEELICENSE_EnableDebugMsg(1);
    AEEPRELOAD_EnableDebugMsg(1);
    AEEFASTLOAD_EnableDebugMsg(1);
    AEEMOD_EnableDebugMsg(1);
    AEESERVICE_EnableDebugMsg(1);
    AEEDISPATCH_EnableDebugMsg(1);
    AEEALARMS_EnableDebugMsg(1);
    AEEEVENT_EnableDebugMsg(1);
    AEEPRIV_EnableDebugMsg(1);
    AEEKEY_EnableDebugMsg(1);
    AEEAPP_EnableDebugMsg(1);
    AEEDBGKEY_EnableDebugMsg(1);
    AEEFREEMEM_EnableDebugMsg(1);
    AEESTACK_EnableDebugMsg(1);
#endif

    IShell *pIShell = AEE_Init(0);
    if(pIShell) {
        __android_log_print(ANDROID_LOG_INFO, "BREWEmulatorAndroid", "AEE Initialized");
    } else {
        __android_log_print(ANDROID_LOG_INFO, "BREWEmulatorAndroid", "AEE Initialization failed");
    }

    isBREWRunning = true;
}

AVKType translateKeycode(jint keyCode) {
    AVKType avk = AVK_UNDEFINED;
    switch(keyCode) {
        case AKEYCODE_DPAD_DOWN: avk = AVK_DOWN; break;
        case AKEYCODE_DPAD_UP: avk = AVK_UP; break;
        case AKEYCODE_DPAD_LEFT: avk = AVK_LEFT; break;
        case AKEYCODE_DPAD_RIGHT: avk = AVK_RIGHT; break;

        case AKEYCODE_DPAD_CENTER:
        case AKEYCODE_ENTER:
            avk = AVK_SELECT; break;

        case AKEYCODE_CLEAR:
        case AKEYCODE_ESCAPE:
        case AKEYCODE_C:
            avk = AVK_CLR; break;

        case AKEYCODE_MENU:
        case AKEYCODE_W:
            avk = AVK_SOFT1; break;

        case AKEYCODE_BACK:
        case AKEYCODE_E:
            avk = AVK_SOFT2; break;

        case AKEYCODE_X:
        case AKEYCODE_ENDCALL:
            avk = AVK_END; break;

        case AKEYCODE_Z:
        case AKEYCODE_CALL:
            avk = AVK_SEND; break;

        case AKEYCODE_0:
        case AKEYCODE_NUMPAD_0:
            avk = AVK_0; break;

        case AKEYCODE_1:
        case AKEYCODE_NUMPAD_1:
            avk = AVK_1; break;

        case AKEYCODE_2:
        case AKEYCODE_NUMPAD_2:
            avk = AVK_2; break;

        case AKEYCODE_3:
        case AKEYCODE_NUMPAD_3:
            avk = AVK_3; break;

        case AKEYCODE_4:
        case AKEYCODE_NUMPAD_4:
            avk = AVK_4; break;

        case AKEYCODE_5:
        case AKEYCODE_NUMPAD_5:
            avk = AVK_5; break;

        case AKEYCODE_6:
        case AKEYCODE_NUMPAD_6:
            avk = AVK_6; break;

        case AKEYCODE_7:
        case AKEYCODE_NUMPAD_7:
            avk = AVK_7; break;

        case AKEYCODE_8:
        case AKEYCODE_NUMPAD_8:
            avk = AVK_8; break;

        case AKEYCODE_9:
        case AKEYCODE_NUMPAD_9:
            avk = AVK_9; break;

        case AKEYCODE_STAR:
        case AKEYCODE_S:
        case AKEYCODE_NUMPAD_MULTIPLY:
            avk = AVK_STAR; break;

        case AKEYCODE_POUND:
        case AKEYCODE_D:
        case AKEYCODE_NUMPAD_DIVIDE:
            avk = AVK_POUND; break;
    }

    return avk;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuKeyUp(JNIEnv *env, jobject thiz, jint keyCode) {
    AVKType avk = translateKeycode(keyCode);
    if(avk == AVK_UNDEFINED) {
        return false;
    }
    AEE_KeyRelease(avk);
    return true;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuKeyDown(JNIEnv *env, jobject thiz, jint keyCode) {
    AVKType avk = translateKeycode(keyCode);
    if(avk == AVK_UNDEFINED) {
        return false;
    }
    AEE_Key(avk);
    AEE_KeyPress(avk);
    return true;
}

extern "C"
JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuJNIShutdown(JNIEnv *env, jobject thiz) {
    if(isBREWRunning) {
        isBREWRunning = false;
        AEE_Exit();
        breGfxDestroy();
    }
}