#include <AEEShell.h>
#include <AEEIMedia.h>
#include <OEMHeap.h>
#include <AEE_OEM.h>

OBJECT(CMedia) {
    AEEVTBL(IMedia) *pvt;
    uint32 m_nRefs;
};

static uint32 CMedia_AddRef(IMedia *po) {
    CMedia *pMe = (CMedia *)po;

    return(++(pMe->m_nRefs));
}

static uint32 CMedia_Release(IMedia *po) {
    CMedia *pMe = (CMedia *)po;
    if(pMe->m_nRefs) {
        if(--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

static int CMedia_QueryInterface(IMedia *po, AEEIID cls, void **ppo) {
    switch (cls) {
        case AEECLSID_QUERYINTERFACE:
        case AEEIID_IMedia:
        case AEECLSID_MEDIAPMD:
            *ppo = (void *)po;
            CMedia_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

static int CMedia_RegisterNotify(IMedia *p, PFNMEDIANOTIFY f, void *pd) {
    return SUCCESS;
}

static int CMedia_SetMediaParm(IMedia *p, int c, int32 p1, int32 p2) {
    return EUNSUPPORTED;
}

static int CMedia_GetMediaParm(IMedia *p, int c, int32 *pp1, int32 *pp2) {
    return EUNSUPPORTED;
}

static int CMedia_Play(IMedia *p) {
    return EUNSUPPORTED;
}

static int CMedia_Record(IMedia *p) {
    return EUNSUPPORTED;
}

static int CMedia_Stop(IMedia *p) {
    return EUNSUPPORTED;
}

static int CMedia_Seek(IMedia *p, AEEMediaSeek s, int32 t) {
    return EUNSUPPORTED;
}

static int CMedia_Rewind(IMedia *p, int32 t) {
    return EUNSUPPORTED;
}

static int CMedia_FastForward(IMedia *p, int32 t) {
    return EUNSUPPORTED;
}

static int CMedia_SeekFrame(IMedia *p, AEEMediaSeek s, int32 t) {
    return EUNSUPPORTED;
}

static int CMedia_Pause(IMedia *p) {
    return EUNSUPPORTED;
}

static int CMedia_Resume(IMedia *p) {
    return EUNSUPPORTED;
}

static int CMedia_GetTotalTime(IMedia *p) {
    return EUNSUPPORTED;
}

static int CMedia_GetState(IMedia *p, boolean *pb) {
    return MM_STATE_IDLE;
}

static const VTBL(IMedia) gsCMediaFuncs = {CMedia_AddRef,
                                           CMedia_Release,
                                           CMedia_QueryInterface,
                                           CMedia_RegisterNotify,
                                           CMedia_SetMediaParm,
                                           CMedia_GetMediaParm,
                                           CMedia_Play,
                                           CMedia_Record,
                                           CMedia_Stop,
                                           CMedia_Seek,
                                           CMedia_Pause,
                                           CMedia_Resume,
                                           CMedia_GetTotalTime,
                                           CMedia_GetState};

void IMediaPMD_Init(IShell *ps) {

}

int IMediaPMD_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CMedia *pNew;

    *ppObj = NULL;

    if(ClsId == AEECLSID_MEDIAPMD) {
        pNew = (CMedia*)AEE_NewClassEx((IBaseVtbl*)&gsCMediaFuncs,
                                             sizeof(CMedia), TRUE);
        if (!pNew) {
            return ENOMEMORY;
        } else {
            pNew->m_nRefs = 1;

            *ppObj = pNew;
            return AEE_SUCCESS;
        }
    }

    return EUNSUPPORTED;
}