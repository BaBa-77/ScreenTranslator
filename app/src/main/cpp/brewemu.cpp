#include "brewemu.h"

extern "C" {
#include <AEE_OEMDispatch.h>
#include <OEMNotify.h>
#include <AEEConfig.h>
#include <AEEModTable.h>
#include <BREWVersion.h>
#undef EALREADY
}

#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <thread>
#include <AEE_OEM.h>

const char *gExternalFilesDir = nullptr;

ANativeWindow *gNativeWindow;

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

extern "C"
JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuJNIStartup(JNIEnv *env, jobject thiz, jobject surface) {
    gNativeWindow = ANativeWindow_fromSurface(env, surface);

    acquireExternalFilesDir(env, thiz);

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

    IShell *pIShell = AEE_Init(0);
    if(pIShell) {
        __android_log_print(ANDROID_LOG_INFO, "BREWEmulatorAndroid", "AEE Initialized");
    } else {
        __android_log_print(ANDROID_LOG_INFO, "BREWEmulatorAndroid", "AEE Initialization failed");
    }

    // AEE_EnterAppContext(NULL);
    // StartLauncherApp(NULL);
    // CALLBACK_Init(&gCBStartLauncherApp, StartLauncherApp, NULL);
    // AEE_ResumeCallback(&gCBStartLauncherApp,0);
}
