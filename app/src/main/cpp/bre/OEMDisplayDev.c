#include <android/native_window.h>
#include "../bre2/brewemu.h"
#include "assert.h"

#undef EFAILED

#include "OEMDisplayDev.h"
#include "../OEMBitmapFuncs_priv.h"
#include "AEE_OEM.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "../bre2/breGfx.h"
#include "../bre2/breConfig.h"
#include <GLES/gl.h>

static boolean gbInit = 0;
static IBitmap *gpDevBitmap = NULL;
static AEECallback gCallback;

struct IDisplayDev {
    AEEVTBL(IDisplayDev)   *pvt;
    uint32                  nRefs;
    char *framebuffer;
    GLuint texture;
    GLuint vbo;
};

static IDisplayDev *gOEMDisplayDev = NULL;

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

    if(gOEMDisplayDev) {
        *ppif = gOEMDisplayDev;
        return SUCCESS;
    }

    pMe = (IDisplayDev*)MALLOC(sizeof(IDisplayDev));
    if (NULL == pMe) {
        return ENOMEMORY;
    }

    // ANativeWindow_setBuffersGeometry(gNativeWindow, 0, 0, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);
    int width = BRE_DISPLAY_CONFIG_WIDTH;// ANativeWindow_getWidth(gNativeWindow);
    int height = BRE_DISPLAY_CONFIG_HEIGHT;//ANativeWindow_getHeight(gNativeWindow);

    pMe->pvt = (AEEVTBL(IDisplayDev) *)&gOEMDisplayDevFuncs;
    pMe->nRefs = 1;
    pMe->framebuffer = malloc(width * height * 2);
    MEMSET(pMe->framebuffer, 0xFF, width * height * 2);
    pMe->texture = 0;

    *ppif = pMe;
    gOEMDisplayDev = pMe;
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

        int width = BRE_DISPLAY_CONFIG_WIDTH;
        int height = BRE_DISPLAY_CONFIG_HEIGHT;

        nErr = OEMBitmap16_NewEx(width, height, pDispDev->framebuffer, NULL, pDispDev, (IBitmap**)ppif);
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

    nErr = OEMBitmap16_New_Child(pDevBitmap, NULL, (IBitmap**)ppif);
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

struct OEMDisplayDev_BufferDataStruct {
    float x, y;
    float u, v;
};

static uint32 gLastFrameDrawTime = 0;

int breOemDpyUpdate() {
    IDisplayDev *pMe = gOEMDisplayDev;

    if(!pMe) return SUCCESS;

    int nativeWidth = ANativeWindow_getWidth(gNativeWindow);
    int nativeHeight = ANativeWindow_getHeight(gNativeWindow);

    breGfxAcqCx();

    glViewport(0, 0, nativeWidth, nativeHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float xScalingRatio = (float) nativeWidth / (float) BRE_DISPLAY_CONFIG_WIDTH;
    float yScalingRatio = (float) nativeHeight / (float) BRE_DISPLAY_CONFIG_HEIGHT;

    float scalingRatio = yScalingRatio;
    if(xScalingRatio < yScalingRatio) {
        scalingRatio = xScalingRatio;
    }

    if(!pMe->texture) {
        glGenTextures(1, &pMe->texture);
        glBindTexture(GL_TEXTURE_2D, pMe->texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BRE_DISPLAY_CONFIG_WIDTH, BRE_DISPLAY_CONFIG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    if(!pMe->vbo) {
        glGenBuffers(1, &pMe->vbo);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0.0f, nativeWidth, nativeHeight, 0.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pMe->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BRE_DISPLAY_CONFIG_WIDTH, BRE_DISPLAY_CONFIG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pMe->framebuffer);

    struct OEMDisplayDev_BufferDataStruct *bdata = MALLOC(sizeof(struct OEMDisplayDev_BufferDataStruct) * 6);
    bdata[0].x = 0.0f;
    bdata[0].y = 0.0f;
    bdata[0].u = 0.0f;
    bdata[0].v = 0.0f;

    bdata[1].x = 0.0f;
    bdata[1].y = BRE_DISPLAY_CONFIG_HEIGHT * scalingRatio;
    bdata[1].u = 0.0f;
    bdata[1].v = 1.0f;

    bdata[2].x = BRE_DISPLAY_CONFIG_WIDTH * scalingRatio;
    bdata[2].y = 0.0f;
    bdata[2].u = 1.0f;
    bdata[2].v = 0.0f;

    bdata[3].x = BRE_DISPLAY_CONFIG_WIDTH * scalingRatio;
    bdata[3].y = 0.0f;
    bdata[3].u = 1.0f;
    bdata[3].v = 0.0f;

    bdata[4].x = 0.0f;
    bdata[4].y = BRE_DISPLAY_CONFIG_HEIGHT * scalingRatio;
    bdata[4].u = 0.0f;
    bdata[4].v = 1.0f;

    bdata[5].x = BRE_DISPLAY_CONFIG_WIDTH * scalingRatio;
    bdata[5].y = BRE_DISPLAY_CONFIG_HEIGHT * scalingRatio;
    bdata[5].u = 1.0f;
    bdata[5].v = 1.0f;

    glBindBuffer(GL_ARRAY_BUFFER, pMe->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct OEMDisplayDev_BufferDataStruct) * 6, bdata, GL_DYNAMIC_DRAW);

    FREE(bdata);

    glVertexPointer(2, GL_FLOAT, sizeof(struct OEMDisplayDev_BufferDataStruct), (const void *) offsetof(struct OEMDisplayDev_BufferDataStruct, x));
    glTexCoordPointer(2, GL_FLOAT, sizeof(struct OEMDisplayDev_BufferDataStruct), (const void *) offsetof(struct OEMDisplayDev_BufferDataStruct, u));
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if(BRE_DISPLAY_CONFIG_FRAMES) {
        uint32 time = GETUPTIMEMS();
        uint32 gNextFrameDrawTime = gLastFrameDrawTime + (1000 / BRE_DISPLAY_CONFIG_FRAMES);
        if (time < gNextFrameDrawTime) {
            MSLEEP(gNextFrameDrawTime - time);
        }
        gLastFrameDrawTime = GETUPTIMEMS();
    }

    breGfxSwap();


    return SUCCESS;
}

static int OEMDisplayDev_Update(IDisplayDev *pMe, IBitmap *pbmSrc, AEERect *prc) {
    if(pMe != gOEMDisplayDev) {
        return EUNSUPPORTED;
    }

    return breOemDpyUpdate();
}

