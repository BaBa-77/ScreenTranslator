#include <android/log.h>

#include "../OEMFeatures.h"
#include "OEMDebug.h"
#include "AEE_OEMEvent.h"
#include "AEEStdLib.h"

void OEMDebug_MsgPut(int nLevel, const char *cpszFile, int nLine, const char *cpszMsg) {
    int aLevel = ANDROID_LOG_INFO;
    switch(nLevel) {
        case DBGPRINTF_LEVEL_LOW:
            aLevel = ANDROID_LOG_DEBUG;
            break;
        case DBGPRINTF_LEVEL_MED:
            aLevel = ANDROID_LOG_INFO;
            break;
        case DBGPRINTF_LEVEL_FATAL:
            aLevel = ANDROID_LOG_FATAL;
            break;
        case DBGPRINTF_LEVEL_HIGH:
            aLevel = ANDROID_LOG_WARN;
            break;
        case DBGPRINTF_LEVEL_ERROR:
            aLevel = ANDROID_LOG_ERROR;
            break;
    }

    __android_log_print(aLevel, "BREWEmulator", "dbgprintf(%s:%d): %s", cpszFile, nLine, cpszMsg);
}


void OEMDebug_LogEvent(AEEEvent evt, AEECLSID cls, uint32 pl) {
    DBGPRINTF( "DBGEvent=0x%X cls=0x%X pl=0x%X", evt, cls, pl );
}

void OEMDebug_Trigger(unsigned uDebugTriggerNum) {
    // has no effect
}
