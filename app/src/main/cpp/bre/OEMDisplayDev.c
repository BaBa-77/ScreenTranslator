#include <android/native_window.h>
#include "../brewemu.h"
#include "assert.h"

#undef EFAILED

#include "OEMDisplayDev.h"
#include "../OEMBitmapFuncs_priv.h"
#include "AEE_OEM.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "../bre2/breDisplayConfig.h"

static boolean gbInit = 0;
static IBitmap *gpDevBitmap = NULL;
static AEECallback gCallback;

struct IDisplayDev {
    AEEVTBL(IDisplayDev)   *pvt;
    uint32                  nRefs;
    char *framebuffer;
};


static void OEMBitmapDev_Init(IShell *ps);
static void OEMBitmapDev_Cleanup(void *pData);

static int OEMDisplayDev_New(IShell * piShell, AEECLSID cls, void **ppif);
static int OEMBitmapDev_New(IShell * piShell, AEECLSID cls, void **ppif);
static int OEMBitmapDevChild_New(IShell * piShell, AEECLSID cls, void **ppif);
static int OEMSysFont_New(IShell * piShell, AEECLSID cls, void **ppif);

const AEEStaticClass gOEMDisplayDevClasses[] = {
        {AEECLSID_DEVBITMAP1,      ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDev_New},
        // {AEECLSID_DEVBITMAP2,      ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDev_New},
        // {AEECLSID_DEVBITMAP3,      ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDev_New},
        // {AEECLSID_DEVBITMAP4,      ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDev_New},
        {AEECLSID_DEVBITMAP1_CHILD,ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDevChild_New},
        // {AEECLSID_DEVBITMAP2_CHILD,ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDevChild_New},
        // {AEECLSID_DEVBITMAP3_CHILD,ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDevChild_New},
        // {AEECLSID_DEVBITMAP4_CHILD,ASCF_UPGRADE, PL_SYSTEM,  0, OEMBitmapDevChild_New},
        {AEECLSID_DISPLAYDEV1,     ASCF_UPGRADE, PL_SYSTEM,  0, OEMDisplayDev_New},
        // {AEECLSID_DISPLAYDEV2,     ASCF_UPGRADE, PL_SYSTEM,  0, OEMDisplayDev_New},
        // {AEECLSID_DISPLAYDEV3,     ASCF_UPGRADE, PL_SYSTEM,  0, OEMDisplayDev_New},
        // {AEECLSID_DISPLAYDEV4,     ASCF_UPGRADE, PL_SYSTEM,  0, OEMDisplayDev_New},
        {AEECLSID_FONTSYSNORMAL,   ASCF_UPGRADE, 0,          0, OEMSysFont_New},
        {AEECLSID_FONTSYSLARGE,    ASCF_UPGRADE, 0,          0, OEMSysFont_New},
        {AEECLSID_FONTSYSBOLD,     ASCF_UPGRADE, 0,          0, OEMSysFont_New},
        {0, 0, 0, 0, NULL}
};

static uint32 OEMDisplayDev_AddRef(IDisplayDev *pMe);
static uint32 OEMDisplayDev_Release(IDisplayDev *pMe);
static int OEMDisplayDev_QueryInterface(IDisplayDev *pMe, AEECLSID clsid, void **ppNew);
static int OEMDisplayDev_Update(IDisplayDev *pMe, IBitmap *pbmSrc, AEERect *prc);

static const VTBL(IDisplayDev) gOEMDisplayDevFuncs = {
        OEMDisplayDev_AddRef,
        OEMDisplayDev_Release,
        OEMDisplayDev_QueryInterface,
        OEMDisplayDev_Update,
};


extern void OEMBitmapDev_Init(IShell *ps)
{
    if (!gbInit) {
        CALLBACK_Init(&gCallback, OEMBitmapDev_Cleanup, NULL);
        ISHELL_RegisterSystemCallback(ps, &gCallback, AEE_SCB_AEE_EXIT);
        gbInit = TRUE;
    }
}

extern void OEMBitmapDev_Cleanup(void *pData)
{
    if (gpDevBitmap) {
        IBITMAP_Release(gpDevBitmap);
        gpDevBitmap = NULL;
    }
    gbInit = FALSE;
}

extern int OEMDisplayDev_New(IShell * piShell, AEECLSID cls, void **ppif)
{
    IDisplayDev   *pMe;

    *ppif = NULL;

    if (AEECLSID_DISPLAYDEV1 != cls) {
        return ECLASSNOTSUPPORT;
    }

    pMe = (IDisplayDev*)MALLOC(sizeof(IDisplayDev));
    if (NULL == pMe) {
        return ENOMEMORY;
    }

    ANativeWindow_setBuffersGeometry(gNativeWindow, 0, 0, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);
    int width = BRE_DISPLAY_CONFIG_WIDTH;// ANativeWindow_getWidth(gNativeWindow);
    int height = BRE_DISPLAY_CONFIG_HEIGHT;//ANativeWindow_getHeight(gNativeWindow);

    pMe->pvt = (AEEVTBL(IDisplayDev) *)&gOEMDisplayDevFuncs;
    pMe->nRefs = 1;
    pMe->framebuffer = malloc(width * height * 4);
    MEMSET(pMe->framebuffer, 0xFF, width * height * 4);

    *ppif = pMe;
    return SUCCESS;
}

// global device bitmap
extern int OEMBitmapDev_New(IShell * piShell, AEECLSID cls, void **ppif)
{
    IDisplayDev   *pDispDev = NULL;
    int            nErr = SUCCESS;

    *ppif = NULL;

    if (AEECLSID_DEVBITMAP1 != cls) {
        return ECLASSNOTSUPPORT;
    }

    OEMBitmapDev_Init(piShell);

    if (!gpDevBitmap) {
        nErr = AEE_CreateInstanceSys(AEECLSID_DISPLAYDEV1, (void**)&pDispDev);
        if (SUCCESS != nErr) {
            return nErr;
        }

        int width = BRE_DISPLAY_CONFIG_WIDTH;//ANativeWindow_getWidth(gNativeWindow);
        int height = BRE_DISPLAY_CONFIG_HEIGHT;//ANativeWindow_getHeight(gNativeWindow);

        nErr = OEMBitmap24_NewEx(width, height, pDispDev->framebuffer, NULL, pDispDev, (IBitmap**)ppif);
        if (SUCCESS != nErr) {
            goto Error;
        }

        gpDevBitmap = (IBitmap*)*ppif;
    } else {
        *ppif = gpDevBitmap;
    }

    IBITMAP_AddRef(((IBitmap*)*ppif));

    Error:
    if (pDispDev) {
        IDISPLAYDEV_Release(pDispDev);
    }
    return nErr;
}

// child device bitmap
extern int OEMBitmapDevChild_New(IShell * piShell, AEECLSID cls, void **ppif)
{
    IBitmap    *pDevBitmap = NULL;
    ACONTEXT   *pac;
    int         nErr;

    *ppif = NULL;

    if (AEECLSID_DEVBITMAP1_CHILD != cls) {
        return ECLASSNOTSUPPORT;
    }

    pac = AEE_EnterAppContext(NULL);
    nErr = AEE_CreateInstanceSys(AEECLSID_DEVBITMAP1, (void**)&pDevBitmap);
    AEE_LeaveAppContext(pac);
    if (SUCCESS != nErr) {
        return nErr;
    }

    nErr = OEMBitmap24_New_Child(pDevBitmap, NULL, (IBitmap**)ppif);
    if (SUCCESS != nErr) {
        goto Error;
    }

    Error:
    if (pDevBitmap) {
        IBITMAP_Release(pDevBitmap);
    }
    return nErr;
}


static int OEMSysFont_New(IShell * piShell, AEECLSID cls, void **ppif)
{
    switch (cls) {
        case AEECLSID_FONTSYSNORMAL:
            return ISHELL_CreateInstance(piShell, AEECLSID_FONT_BASIC11, (void **)ppif);
        case AEECLSID_FONTSYSBOLD:
            return ISHELL_CreateInstance(piShell, AEECLSID_FONT_BASIC11B, (void **)ppif);
        case AEECLSID_FONTSYSLARGE:
            return ISHELL_CreateInstance(piShell, AEECLSID_FONT_BASIC14, (void **)ppif);
        default:
            return ECLASSNOTSUPPORT;
    }
}


static uint32 OEMDisplayDev_AddRef(IDisplayDev *pMe)
{
    return ++pMe->nRefs;
}

static uint32 OEMDisplayDev_Release(IDisplayDev *pMe)
{
    uint32 nRefs = (pMe->nRefs ? --pMe->nRefs : 0);

    if (!nRefs) {
        free(pMe->framebuffer);

        FREE(pMe);
    }

    return nRefs;
}

static int OEMDisplayDev_QueryInterface(IDisplayDev *pMe, AEECLSID clsid, void **ppNew)
{
    if (clsid == AEEIID_DISPLAYDEV || clsid == AEECLSID_QUERYINTERFACE) {
        *ppNew = (void*)pMe;
        OEMDisplayDev_AddRef(pMe);
        return SUCCESS;
    }

    *ppNew = 0;
    return ECLASSNOTSUPPORT;
}

static int OEMDisplayDev_Update(IDisplayDev *pMe, IBitmap *pbmSrc, AEERect *prc)
{
    DBGPRINTF("OEMDisplayDev_Update");

    ANativeWindow_Buffer buffer;
    ARect updateRect;
    updateRect.left = prc->x;
    updateRect.top = prc->y;
    updateRect.right = prc->x + prc->dx;
    updateRect.bottom = prc->y + prc->dy;
    int error_code = ANativeWindow_lock(gNativeWindow, &buffer, &updateRect);
    if(error_code != 0) {
        return EFAILED;
    }

    assert(buffer.format == AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);

    char *outFB = (char *) buffer.bits;

    // pMe->framebuffer[0] = 255;
    // pMe->framebuffer[1] = 255;
    // pMe->framebuffer[2] = 255;

    // MEMSET(pMe->framebuffer, 0xFF, buffer.width * 4 * 5);

    for(int x = updateRect.left; x < updateRect.right && x < BRE_DISPLAY_CONFIG_WIDTH; x++) {
        for(int y = updateRect.top; y < updateRect.bottom && y < BRE_DISPLAY_CONFIG_HEIGHT; y++) {
            int inOffset = (x + y * BRE_DISPLAY_CONFIG_WIDTH) * 4;
            int outOffset = (x + y * buffer.stride) * 4;
            char r = pMe->framebuffer[inOffset + 0];
            char g = pMe->framebuffer[inOffset + 1];
            char b = pMe->framebuffer[inOffset + 2];

            outFB[outOffset + 0] = r;
            outFB[outOffset + 1] = g;
            outFB[outOffset + 2] = b;
            outFB[outOffset + 3] = 0xFF;
        }
    }

    ANativeWindow_unlockAndPost(gNativeWindow);

    return SUCCESS;
}

