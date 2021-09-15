#include "OEMVersion.h"
#include "AEE.h"
#include "AEEStdLib.h"
#include "OEMNotify.h"
#include "AEE_OEMDispatch.h"
#include "AEE_OEMDownload.h"
#include "AEEConfig.h"
#include <AEE_OEM.h>

#include "../bre2/breStartup.h"
#include "../bre2/breConfig.h"
#include "../brewappmgr/BrewAppMgr.bid"

static AEECallback gCBResetBREW;
static boolean gbInResetBREW = FALSE;
static AEECallback gCBStartLauncherApp;

static void ResetBREW(void *pUnused) {
    //Set this bool. Calling AEE_Exit() will result in NTF_IDLE. We do not want AutoStartApp to
    //be started there.
    gbInResetBREW = TRUE;

    AEE_Exit();

    gbInResetBREW = FALSE;

    //AEE_Init will also take care of starting AutoStart App
    AEE_Init(0);
}

int OEM_Notify(OEMNotifyEvent evt, uint32 dw) {
    OEMAppEvent *pae = (OEMAppEvent *) dw;

    switch (evt) {
        case OEMNTF_MODLOAD: {
            OEMNotifyModLoad *pML = (OEMNotifyModLoad *) dw;

            if ((void *) 0 == pML->pLocation) {
                return EFAILED;
            }
            return SUCCESS;
        }

        case OEMNTF_IDLE:
            if (gbInResetBREW)
                break;
            CALLBACK_Init(&gCBStartLauncherApp, StartLauncherApp, NULL);
            AEE_ResumeCallback(&gCBStartLauncherApp,0);
            break;

        case OEMNTF_RESET_BREW:
            CALLBACK_Init(&gCBResetBREW, ResetBREW, NULL);
            AEE_ResumeCallback(&gCBResetBREW, NULL);
            break;

        case OEMNTF_RESET:
            break;
        case OEMNTF_APP_EVENT:
            if (pae->evt == EVT_APP_START ||
                pae->evt == EVT_APP_STOP ||
                pae->evt == EVT_APP_SUSPEND ||
                pae->evt == EVT_APP_RESUME) {
                DBGEVENT(pae->evt, pae->cls);
            }

            break;
        case OEMNTF_APP_CTXT_NEW:    // fall through
        case OEMNTF_APP_CTXT_DELETE: // fall through
            break;

        case OEMNTF_APP_CTXT_SWITCH:
            break;
        default:
            break;
    }
    return (0);
}

boolean OEM_IsClsOKInSafeMode(uint32 clsid)
{
    return TRUE;
}

boolean OEM_SimpleBeep(BeepType nBeepType, boolean bLoud)
{
    return TRUE;
}

int OEM_GetUpdateVersion(char *pszVersion, int *pnLen) {
    int nUpdateVerLen = 0;

    if (0 == pnLen) {
        return EFAILED;
    }

    nUpdateVerLen = STRLEN(BREW_UPDATE_VERSION) + 1;

    if (0 == pszVersion) {
        *pnLen = nUpdateVerLen - 1; // not include null termination.
        return SUCCESS;
    }

    if (*pnLen < nUpdateVerLen) {
        *pnLen = nUpdateVerLen - 1; // not include null termination.
        *pszVersion = 0;
        return EFAILED;
    }

    STRCPY(pszVersion, BREW_UPDATE_VERSION);
    *pnLen = nUpdateVerLen - 1;  // not include null termination.

    return SUCCESS;
}

void OEM_AuthorizeDownload(IDownload * pd, DLITEMID iID, DLPRICEID iPrice,DLItem * pItem,PFNCHECKDLCB pfn, void * pUser)
{
    pfn(pUser,iID,iPrice,0);
}

int OEM_SetConfig(AEEConfigItem i, void * pBuff, int nSize) {
    return EUNSUPPORTED;
}

static void GetMobileInfo(AEEMobileInfo * pMobileInfo) {
    pMobileInfo->nCurrNAM = 0;
    pMobileInfo->dwESN = BRE_CONFIG_ESN;
    strcpy(pMobileInfo->szMobileID, BRE_CONFIG_IMEI);
}

int OEM_GetConfig(AEEConfigItem i, void * pBuff, int nSize)
{
    switch(i) {
        case CFGI_MOBILEINFO:
            if(!pBuff || nSize != sizeof(AEEMobileInfo))
                return(EBADPARM);

            GetMobileInfo((AEEMobileInfo *)pBuff);
            return(0);
        case CFGI_AUTOSTART: {
            AEECLSID * pc = (AEECLSID *)pBuff;

            if(nSize != sizeof(AEECLSID))
                return(EBADPARM);

            // *pc = 0x01035893;// 0x01009FF0;
            *pc = AEECLSID_BREWAPPMGR_BID;//0x010719f9;
            // *pc = 0x01009FF0;

            return SUCCESS;
        }
    }
    return EUNSUPPORTED;
}

void OEM_SetBAM_ADSAccount(void) {
}

#include "../brewemu.h"
#include <android/native_window.h>

void OEM_GetDeviceInfo(AEEDeviceInfo * pi) {
    // TODO:

    pi->cxScreen = BRE_DISPLAY_CONFIG_WIDTH;
    pi->cyScreen = BRE_DISPLAY_CONFIG_HEIGHT;
    pi->cxAltScreen = 0;
    pi->cyAltScreen = 0;
    pi->cxScrollBar = 8;
    pi->wEncoding = AEE_ENC_UTF8;
    pi->wMenuTextScroll = 200;
    pi->nColorDepth = 24;
    pi->wMenuImageDelay = 1000;
    pi->dwRAM = BRE_CONFIG_HEAP_SIZE;
    pi->bAltDisplay = FALSE;
    pi->bFlip = FALSE;
    pi->bVibrator = TRUE;
    pi->bExtSpeaker = TRUE;
    pi->bVR = FALSE;
    pi->bPosLoc = FALSE;
    pi->bMIDI = FALSE;
    pi->bCMX = FALSE;
    pi->bPen = TRUE;
    pi->dwPromptProps = 0;
    pi->wKeyCloseApp = AVK_END;
    pi->wKeyCloseAllApps = 0;
    pi->dwLang = 0;
    pi->wStructSize = offsetof(AEEDeviceInfo, dwNetLinger);
}

static const RGBVAL gSystemColors[] = {
    0,
    0x000000FF, // CLR_USER_TEXT
    0xFFFFFFFF, // CLR_USER_BACKGROUND
    0x2c3e50ff, // CLR_USER_LINE
    0x000000ff, // CLR_SYS_TITLE
    0xFFFFFFFF, // CLR_SYS_TITLE_TEXT
    0xecf0f1ff, // CLR_SYS_ITEM
    0x000000FF, // CLR_SYS_ITEM_TEXT
    0x2980b9FF, // CLR_SYS_ITEM_SEL
    0xFFFFFFFF, // CLR_SYS_ITEM_SEL_TEXT
    0xbdc3c7FF, // CLR_SYS_WIN
    0xFFFFFFFF, // CLR_SYS_FRAME_HI
    0x000000FF, // CLR_SYS_FRAME_LO
    0xCCCCCCFF, // CLR_SYS_LT_SHADOW
    0x444444FF, // CLR_SYS_DK_SHADOW
    0x95a5a6FF, // CLR_SYS_SCROLLBAR
    0x34495eFF, // CLR_SYS_SCROLLBAR_FILL,
    0 // CLR_SYS_LAST
};

int OEM_GetDeviceInfoEx(AEEDeviceItem nItem, void *pBuff, int *pnSize) {
    switch(nItem) {
        case AEE_DEVICEITEM_SYS_COLORS_DISP1: {
            int nSize;

            if(!pnSize)
                return(EBADPARM);

            nSize = MIN(*pnSize, CLR_SYS_LAST * sizeof(RGBVAL));
            *pnSize = CLR_SYS_LAST * sizeof(RGBVAL);

            if (NULL == pBuff) {
                return SUCCESS;
            }

            MEMCPY(pBuff, gSystemColors, nSize);

            return SUCCESS;
        }
    }
    return EUNSUPPORTED;
}
