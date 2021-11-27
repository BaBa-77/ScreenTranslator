#include "OEMTAPI.h"

#include <string.h>
#include <stdlib.h>
#include <AEEStdLib.h>
#include <OEMHeap.h>
#include <assert.h>
#include "AEE_OEM.h"
#include "../bre2/breConfig.h"

struct _OEMTAPI  {
    uint32 dwRefs;
};

static OEMTAPI *gpTAPI = NULL;

int OEMTAPI_New( OEMTAPI **ppif ) {
    int nErr = SUCCESS;

    if (gpTAPI) {
        ++gpTAPI->dwRefs;
        *ppif = gpTAPI;
        return SUCCESS;
    }

    gpTAPI = (OEMTAPI *)sys_malloc(sizeof(OEMTAPI));
    if(!gpTAPI) {
        return ENOMEMORY;
    }

    ZEROAT(gpTAPI);
    gpTAPI->dwRefs = 1;
    *ppif = gpTAPI;

    return nErr;
}

int OEMTAPI_Destroy( OEMTAPI * pme ) {
    if(--pme->dwRefs) {
        return SUCCESS;
    }

    if( pme == gpTAPI ) {
        gpTAPI = NULL;
    }
    sys_free(pme);

    return SUCCESS;
}

int OEMTAPI_GetStatus( OEMTAPI * po, TAPIStatus * ps) {
    memset(ps, 0, sizeof(TAPIStatus));
    breGetConfigEntry(BRE_CFGE_IMEI, &ps->szMobileID);
    ps->state = PS_OFFLINE;
    return SUCCESS;
}

int OEMTAPI_MakeVoiceCall(OEMTAPI * po, const char * pszNumber, AEECallback *pcbDial) {
    return EBADPARM;
}

boolean OEMTAPI_GetCallerID(OEMTAPI * po, AECHAR * psz, int nSize) {
    return FALSE;
}

int OEMTAPI_OnCallStatus(OEMTAPI * po, AEECallback *pcb) {
    return SUCCESS;
}

boolean OEMTAPI_IsVoiceCall(OEMTAPI * po) {
    return FALSE;
}

boolean OEMTAPI_IsDataSupported(OEMTAPI * po) {
    return FALSE;
}

uint16 OEMTAPI_GetCallStatus( OEMTAPI *po ) {
    return 0;
}

int OEMTAPI_OnPhoneStatus(OEMTAPI * po, AEECallback *pcb) {
    return SUCCESS;
}

byte *AEE_GetDecodedTextString(byte *pText, uint32 nText, int se, int *ae, uint32 *pnBytes) {
    assert(FALSE);
}
