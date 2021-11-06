#include <AEE_OEM.h>
#include <OEMHeap.h>
#include "AEEMediaUtil.h"
#include "OEMClassIDs.h"

OBJECT(CAEEMediaUtil) {
        AEEVTBL(IMediaUtil) *pvt;
        uint32 m_nRefs;
};

static uint32 AEEMediaUtil2_AddRef(IMediaUtil * po) {
    CAEEMediaUtil *pMe = (CAEEMediaUtil *)po;

    return(++(pMe->m_nRefs));
}

static uint32 AEEMediaUtil2_Release(IMediaUtil * po) {
    CAEEMediaUtil *pMe = (CAEEMediaUtil *)po;
    if(pMe->m_nRefs) {
        if(--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

static int AEEMediaUtil2_QueryInterface(IMediaUtil *po, AEECLSID clsid, void **ppNew)
{
    if (clsid == AEECLSID_MEDIAUTIL || clsid == AEECLSID_QUERYINTERFACE) {
        *ppNew = (void*)po;
        (void) AEEMediaUtil2_AddRef(po);
        return SUCCESS;
    }

    return ECLASSNOTSUPPORT;
}

static int AEEMediaUtil2_CreateMedia(IMediaUtil * po, AEEMediaData * pmd, IMedia ** ppm) {
    if(ppm) *ppm = NULL;
    return EUNSUPPORTED;
}

static int AEEMediaUtil2_EncodeMedia(IMediaUtil * po, AEEMediaEncodeResult * per, AEECLSID clsDest, AEEMediaEncodeInfo * pei, AEECallback * pcb) {
    // if(ppm) *ppm = NULL;
    return EUNSUPPORTED;
}

static int AEEMediaUtil2_CreateMediaEx(IMediaUtil * po, AEEMediaCreateInfo * pcmi, IMedia ** ppm) {
    if(ppm) *ppm = NULL;
    return EUNSUPPORTED;
}


static const VTBL(IMediaUtil) gsAEEMediaUtilFuncs ={ AEEMediaUtil2_AddRef,
                                                     AEEMediaUtil2_Release,
                                                     AEEMediaUtil2_QueryInterface,
                                                     AEEMediaUtil2_CreateMedia,
                                                     AEEMediaUtil2_EncodeMedia,
                                                     AEEMediaUtil2_CreateMediaEx,
                                                     };


int IMediaUtil_New(IShell *pIShell, AEECLSID ClsId, void **ppObj) {
    CAEEMediaUtil *pNew;

    *ppObj = NULL;

    if(ClsId == AEECLSID_MEDIAUTIL) {
        pNew = (CAEEMediaUtil*)AEE_NewClassEx((IBaseVtbl*)&gsAEEMediaUtilFuncs,
                                             sizeof(CAEEMediaUtil), TRUE);
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
