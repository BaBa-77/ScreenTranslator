#include <pthread.h>
#include "OEMCriticalSection.h"

typedef struct {
    boolean bInit;
    pthread_mutex_t crit_sect;
} CriticalSection;

// Translate the OEMCriticalSection handle the user gave us into a
// CriticalSection pointer.  This includes initializing it the first
// time.  This looks like it is doing a lot of work, but there is
// actually not much to it the nominal case - it is mostly handling
// the exception cases.
static __inline CriticalSection* CriticalSection_FromOEMCriticalSection(OEMCriticalSection* pm) {
    CriticalSection* me = (CriticalSection*)pm;

    // Initialize once, in a thread safe manner
    if (FALSE == me->bInit) {
        dword dwSave = 0;

        if (FALSE == me->bInit) {
            pthread_mutexattr_t attr;

            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&me->crit_sect, &attr);
            me->bInit = TRUE;
        }
    }

    return me;
}

/*=====================================================================
Function: OEMCriticalSection_Enter

Description:
   Enters a critical_section.  Sleeps if another task already in the
   critical section.
=====================================================================*/
void OEMCriticalSection_Enter(OEMCriticalSection* pm)
{
    CriticalSection* me = CriticalSection_FromOEMCriticalSection(pm);

    pthread_mutex_lock(&me->crit_sect);
}

/*=====================================================================
Function: OEMCriticalSection_Leave

Description:
   Leaves a critical section.  Awakes and task waiting to enter the
   critical section
=====================================================================*/
void OEMCriticalSection_Leave(OEMCriticalSection* pm)
{
    CriticalSection* me = CriticalSection_FromOEMCriticalSection(pm);

    pthread_mutex_unlock(&me->crit_sect);
}

