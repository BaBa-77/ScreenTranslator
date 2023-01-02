extern "C" {
    #include <AEEShell.h>
    #include <AEEIMedia.h>
    #include <OEMHeap.h>
    #include <AEE_OEM.h>
}

#include "../mmapi/MMPlayer.h"
#include <AEEISource.h>

OBJECT(CMedia) {
    AEEVTBL(IMedia) *pvt;
    uint32 m_nRefs;
    MMPlayer *m_player;
};

static uint32 CMedia_AddRef(IMedia *po) {
    CMedia *pMe = (CMedia *)po;

    return(++(pMe->m_nRefs));
}

static uint32 CMedia_Release(IMedia *po) {
    CMedia *pMe = (CMedia *)po;
    if(pMe->m_nRefs) {
        if(--pMe->m_nRefs == 0) {
            delete pMe->m_player;
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
        case AEECLSID_MEDIAMIDI:
        case AEECLSID_MEDIAAAC:
        case AEECLSID_MEDIAQCP:
        case AEECLSID_MEDIAMP3:
        case AEECLSID_MEDIAAMR:
        case AEECLSID_MEDIAPCM:
        case AEECLSID_MEDIAADPCM:
        case AEECLSID_MEDIAIMELODY:
            *ppo = (void *)po;
            CMedia_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

static int CMedia_RegisterNotify(IMedia *p, PFNMEDIANOTIFY f, void *pd) {
    DBGPRINTF("CMedia_RegisterNotify SHIM");
    return SUCCESS;
}

static int CMedia_SetMediaParm(IMedia *p, int nParamID, int32 p1, int32 p2) {
    CMedia *cm = (CMedia *) p;
    if(nParamID == MM_PARM_MEDIA_DATA) {
        auto *data = (AEEMediaData *) p1;
        if(data->clsData == MMD_FILE_NAME) {
            DBGPRINTF("CMedia_SetMediaParm mdata file %s", data->pData);
        } else if(data->clsData == MMD_BUFFER) {
            cm->m_player->setMediaSource(data->pData, data->dwSize);
            return SUCCESS;
        } else if(data->clsData == MMD_ISOURCE) {
            DBGPRINTF("CMedia_SetMediaParm mdata isource");
        }
    } else {
        DBGPRINTF("CMedia_SetMediaParm SHIM %d %d %d", nParamID, p1, p2);
    }
    return EUNSUPPORTED;
}

static int CMedia_GetMediaParm(IMedia *p, int c, int32 *pp1, int32 *pp2) {
    DBGPRINTF("CMedia_GetMediaParm SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Play(IMedia *p) {
    DBGPRINTF("CMedia_Play SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Record(IMedia *p) {
    DBGPRINTF("CMedia_Record SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Stop(IMedia *p) {
    DBGPRINTF("CMedia_Stop SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Seek(IMedia *p, AEEMediaSeek s, int32 t) {
    DBGPRINTF("CMedia_Seek SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Rewind(IMedia *p, int32 t) {
    DBGPRINTF("CMedia_Rewind SHIM");
    return EUNSUPPORTED;
}

static int CMedia_FastForward(IMedia *p, int32 t) {
    DBGPRINTF("CMedia_FastForward SHIM");
    return EUNSUPPORTED;
}

static int CMedia_SeekFrame(IMedia *p, AEEMediaSeek s, int32 t) {
    DBGPRINTF("CMedia_SeekFrame SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Pause(IMedia *p) {
    DBGPRINTF("CMedia_Pause SHIM");
    return EUNSUPPORTED;
}

static int CMedia_Resume(IMedia *p) {
    DBGPRINTF("CMedia_Resume SHIM");
    return EUNSUPPORTED;
}

static int CMedia_GetTotalTime(IMedia *p) {
    DBGPRINTF("CMedia_GetTotalTime SHIM");
    return EUNSUPPORTED;
}

static int CMedia_GetState(IMedia *p, boolean *pb) {
    DBGPRINTF("CMedia_GetState SHIM");
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

extern "C" void IMediaPMD_Init(IShell *ps) {

}

extern "C" int IMediaPMD_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CMedia *pNew;

    *ppObj = NULL;

    if(ClsId == AEECLSID_MEDIAPMD) {
        pNew = (CMedia*)AEE_NewClassEx((IBaseVtbl*)&gsCMediaFuncs,
                                             sizeof(CMedia), TRUE);
        if (!pNew) {
            return ENOMEMORY;
        } else {
            pNew->m_nRefs = 1;
            pNew->m_player = new MMPlayer;
            pNew->m_player->setMediaFormat(MMPlayer::MMF_CMIDI);

            *ppObj = pNew;
            return AEE_SUCCESS;
        }
    }

    return EUNSUPPORTED;
}

extern "C" void IMediaBg_Init(IShell *ps) {

}

extern "C" int IMediaBg_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CMedia *pNew;

    *ppObj = NULL;

    pNew = (CMedia*)AEE_NewClassEx((IBaseVtbl*)&gsCMediaFuncs,
                                   sizeof(CMedia), TRUE);
    if (!pNew) {
        return ENOMEMORY;
    } else {
        pNew->m_nRefs = 1;
        pNew->m_player = new MMPlayer;
        pNew->m_player->setMediaFormat(MMPlayer::MMF_DETECT);

        *ppObj = pNew;
        return AEE_SUCCESS;
    }

    return EUNSUPPORTED;
}

extern "C" void IMediaMain_Init(IShell *ps) {
    IMediaBg_Init(ps);
}

extern "C" int IMediaMain_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    return IMediaBg_New(ps, ClsId, ppObj);
}
