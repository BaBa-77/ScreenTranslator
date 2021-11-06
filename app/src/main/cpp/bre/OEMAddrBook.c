#include <AEE_OEM.h>
#include <OEMHeap.h>
#include "OEMAddrBook.h"
#include "OEMClassIDs.h"

OBJECT(COEMAddrBook) {
    AEEVTBL(IOEMAddrBook) *pvt;
    uint32 m_nRefs;
};

static uint32 OEMAddr_AddRef(IOEMAddrBook * po) {
    COEMAddrBook *pMe = (COEMAddrBook *)po;

    return(++(pMe->m_nRefs));
}

static uint32 OEMAddr_Release(IOEMAddrBook * po) {
    COEMAddrBook *pMe = (COEMAddrBook *)po;
    if(pMe->m_nRefs) {
        if(--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

static int OEMAddr_EnumRecInit(AEEAddrCat wCategory, AEEAddrFieldID wFieldID, void *pData, uint16 wDataSize) {
    return AEE_SUCCESS;
}

static uint16 OEMAddr_EnumNextRec(AEEAddrCat *pcat,AEEAddrField ** ppItems, int *pnItemCount,int *pErr) {
    if(pErr) *pErr = AEE_SUCCESS;
    return AEE_ADDR_RECID_NULL;
}

static int OEMAddr_GetCatCount(void) {
    return 0;
}

static int OEMAddr_GetCatList(AEEAddrCat *p, int nSize) {
    return AEE_SUCCESS;
}

static int OEMAddr_GetFieldInfo(AEEAddrCat c, AEEAddrFieldInfo *pf, int nSize) {
    return AEE_SUCCESS;
}

static int OEMAddr_GetFieldInfoCount(AEEAddrCat c) {
    return 0;
}

static uint16 OEMAddr_GetNumRecs(void) {
    return 0;
}

static uint16 OEMAddr_RecordAdd(AEEAddrCat cat, AEEAddrField * pItems, int nItemCount,int *pErr) {
    if(pErr) *pErr = EFAILED;
    return AEE_ADDR_RECID_NULL;
}

static int OEMAddr_RecordDelete(uint16 wrecID) {
    return EFAILED;
}

static int OEMAddr_RecordGetByID(uint16 wrecID,AEEAddrCat *pcat,AEEAddrField ** ppItems, int *pnItemCount,int *pErr) {
    if(pErr) *pErr = EFAILED;
    return EFAILED;
}

static int OEMAddr_RecordUpdate(uint16 wrecID,AEEAddrCat cat,AEEAddrField * pItems, int nItemCount,int *pErr) {
    if(pErr) *pErr = EFAILED;
    return EFAILED;
}

static int OEMAddr_RemoveAllRecs(void) {
    return EUNSUPPORTED;
}

static void OEMAddr_SetProperties(uint32 dwProps) {

}

static uint32 OEMAddr_GetProperties(void) {
    return 0;
}

static int OEMAddr_GetCategoryName(AEEAddrCat c, AECHAR *pszName, int *pnSize) {
    return AEE_ADDRBOOK_NOCATSUPPORT;
}

static int OEMAddr_GetFieldName(AEEAddrFieldID f, AECHAR *pszFieldName, int *pnSize) {
    return AEE_ADDR_INVALID_FIELD_ID;
}

static const VTBL(IOEMAddrBook) gsOEMAddrBookFuncs ={ OEMAddr_AddRef,
                                                      OEMAddr_Release,
                                                      OEMAddr_EnumNextRec,
                                                      OEMAddr_EnumRecInit,
                                                      OEMAddr_GetCatCount,
                                                      OEMAddr_GetCatList,
                                                      OEMAddr_GetFieldInfo,
                                                      OEMAddr_GetFieldInfoCount,
                                                      OEMAddr_GetNumRecs,
                                                      OEMAddr_RecordAdd,
                                                      OEMAddr_RecordDelete,
                                                      OEMAddr_RecordGetByID,
                                                      OEMAddr_RecordUpdate,
                                                      OEMAddr_RemoveAllRecs,
                                                      OEMAddr_SetProperties,
                                                      OEMAddr_GetProperties,
                                                      OEMAddr_GetCategoryName,
                                                      OEMAddr_GetFieldName};


int OEMAddrBook_New(IShell *pIShell, AEECLSID ClsId, void **ppObj) {
    COEMAddrBook *pNew;

    *ppObj = NULL;

    if(ClsId == AEECLSID_OEMADDRBOOK) {
        pNew = (COEMAddrBook*)AEE_NewClassEx((IBaseVtbl*)&gsOEMAddrBookFuncs,
                                             sizeof(COEMAddrBook), TRUE);
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
