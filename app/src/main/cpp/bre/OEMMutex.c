#include <pthread.h>
#include <stdlib.h>

#include "OEMMutex.h"

OEMMutex OEMMutex_Create(void) {
    pthread_mutex_t *mtx = malloc(sizeof(pthread_mutex_t));
    if (mtx == 0) {
        return (OEMMutex) NULL;
    }

    int err = pthread_mutex_init(mtx, NULL);
    if (err != 0) {
        free(mtx);
        return (OEMMutex) NULL;
    }

    return mtx;
}

void OEMMutex_Lock(OEMMutex m) {
    pthread_mutex_lock((pthread_mutex_t *) m);
}

void OEMMutex_Unlock(OEMMutex m) {
    pthread_mutex_unlock((pthread_mutex_t *) m);
}

void OEMMutex_Destroy(OEMMutex m) {
    pthread_mutex_destroy((pthread_mutex_t *) m);
    free((void *) m);
}
