#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <android/looper.h>
#include <string.h>
#include <sys/fcntl.h>

#include "OEMOS.h"
#include "AEE_OEMDispatch.h"
#include "time_jul.h"

/*static int oemos_timer = -1;
static int oemos_dispatchPipe[2];
static uint32 oemos_lastDispatchTime = 0;

int OEMOS_HandleTimer(int fd, int events, void *data) {
    if(events & ALOOPER_EVENT_INPUT) {
        if(fd == oemos_timer) {
            uint64_t buf;
            int expired = read(fd, &buf, sizeof(uint64_t));
            for (int i = 0; i < expired; i++) {
                AEE_Dispatch();
            }
        } else if(fd == oemos_dispatchPipe[0]) {
            char msg;
            while(1) { // discard dispatches while they are there
                ssize_t numRead = read(fd, &msg, 1);
                if(numRead <= 0) {
                    break;
                }
            }
            AEE_Dispatch();
        }
    }

    return 1;
}*/

int16 OEMOS_InitLayer() {
    /*oemos_timer = syscall(SYS_timerfd_create, CLOCK_MONOTONIC, 04000);
    if(oemos_timer < 0) {
        return EFAILED;
    }

    if(pipe(oemos_dispatchPipe) < 0) {
        return EFAILED;
    }

    fcntl(oemos_dispatchPipe[0], F_SETFL, fcntl(oemos_dispatchPipe[0], F_GETFL) | O_NONBLOCK);

    ALooper *looper = ALooper_forThread();
    ALooper_acquire(looper);
    ALooper_addFd(looper, oemos_timer, ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, &OEMOS_HandleTimer, NULL);
    ALooper_addFd(looper, oemos_dispatchPipe[0], ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, &OEMOS_HandleTimer, NULL);*/

    return AEE_SUCCESS;
}

uint32  OEMOS_JulianToSeconds(JulianType* pDate)
{
    time_julian_type ts;

    ts.day = pDate->wDay;
    ts.day_of_week = pDate->wWeekDay;
    ts.hour = pDate->wHour;
    ts.minute = pDate->wMinute;
    ts.second = pDate->wSecond;
    ts.month = pDate->wMonth;
    ts.year = pDate->wYear;

    return (time_jul_to_secs(&ts));
}

void OEMOS_Breakpoint(uint32 dwType, void * pData, uint32 nSize)
{
    // TODO:
}

uint32 OEMOS_GetUpTime() {
    struct timespec tv;
    static uint32 startTime = UINT32_MAX;
    int status = clock_gettime(CLOCK_MONOTONIC, &tv); // don't use CLOCK_PROCESS_CPUTIME_ID!
                                                      // it's slow and OEMOS_GetUpTime is used as
                                                      // high-precision timer in BREW
    if(status == -1) {
        errno = 0;
        return 0;
    }
    uint32 time = (((uint32) tv.tv_sec) * 1000) + (tv.tv_nsec / 1000000);
    if(startTime == UINT32_MAX) {
        startTime = time;
    }
    return time - startTime;
}

int32 OEMOS_LocalTimeOffset (boolean *bDaylightSavings) {
    // FIXME: i'm not sure about direction of X/Open timezone variable

    if(bDaylightSavings) {
        *bDaylightSavings = daylight != 0;
    }
    return -timezone;
}

uint32 OEMOS_ActiveTaskID() {
    return (uint32)getpid();
}

void OEMOS_Sleep(uint32 nMSecs) {
    struct timespec ts;
    ts.tv_sec = nMSecs / 1000;
    ts.tv_nsec = (nMSecs % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

static uint32 signalsUnlocked = 1;
static sigset_t savedSignalMask;

uint32 OEMOS_IntLock(void) {
    uint32 oldSignalsUnlocked = signalsUnlocked;

    if(oldSignalsUnlocked) {
        sigset_t sigs;

        sigfillset(&sigs);
        sigprocmask(SIG_SETMASK, &sigs, &savedSignalMask);
    }
    signalsUnlocked = 0;

    return oldSignalsUnlocked;
}

void OEMOS_IntFree( uint32 intSav ) {
    if(intSav) {
        sigset_t sigs;

        sigprocmask(SIG_SETMASK, &savedSignalMask, NULL);
    }
    signalsUnlocked = intSav;
}

void OEMOS_ResetDevice(char * pszMsg, uint32 nCause) {
    // TODO:
}