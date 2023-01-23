#include "breConfig.h"

#include "OEMFS.h"
#include <stdlib.h>
#include <limits.h>
#include "toml.h"
#include <string.h>

static int breCfgWidth = 240;
static int breCfgHeight = 320;
static uint32_t breCfgEsn = 0xab2b3c4f;
static char breCfgImei[16] = "123456781234567";
static int breCfgHeapSize = 67108864;
static int breCfgDebugLog = 0;
static int breCfgFrames = 0;
static uint64_t breCfgForcedTime = 0;
static int breCfgKeypad = 1;
static int breCfgRotation = 0;
static int breCfgFlip = 0;
static uint64_t breCfgStorageLimit = 0;

void breInitConfig() {
    char *pathbuf = malloc(PATH_MAX);
    int pathbufSize = PATH_MAX;
    if(SUCCESS == OEMFS_GetNativePath("fs:/sys/breConfig.toml", pathbuf, &pathbufSize)) {
        FILE *fp = fopen(pathbuf, "r");
        if(!fp) {
            goto cleanup;
        }

        char errbuf[200];
        toml_table_t *conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
        fclose(fp);

        if(!conf) {
            goto cleanup;
        }

        toml_datum_t width = toml_int_in(conf, "width");
        if(width.ok) {
            breCfgWidth = (int) width.u.i;
        }

        toml_datum_t height = toml_int_in(conf, "height");
        if(height.ok) {
            breCfgHeight = (int) height.u.i;
        }

        toml_datum_t esn = toml_int_in(conf, "esn");
        if(esn.ok) {
            breCfgEsn = (uint32_t) esn.u.i;
        }

        toml_datum_t imei = toml_string_in(conf, "imei");
        if(imei.ok) {
            strncpy(breCfgImei, imei.u.s, 16);
            free(imei.u.s);
        }

        toml_datum_t heapSize = toml_int_in(conf, "heapSize");
        if(heapSize.ok) {
            breCfgHeapSize = (uint32_t) heapSize.u.i;
        }

        toml_datum_t debugLog = toml_bool_in(conf, "debugLog");
        if(debugLog.ok) {
            breCfgDebugLog = debugLog.u.b;
        }

        toml_datum_t frames = toml_int_in(conf, "frames");
        if(frames.ok) {
            breCfgFrames = (int) frames.u.i;
        }

        toml_datum_t forcedTime = toml_int_in(conf, "forcedTime");
        if(forcedTime.ok) {
            breCfgForcedTime = (uint64_t) forcedTime.u.i;
        }

        toml_datum_t keypad = toml_int_in(conf, "keypad");
        if(keypad.ok) {
            breCfgKeypad = (int) keypad.u.i;
        }

        toml_datum_t rotation = toml_int_in(conf, "rotation");
        if (rotation.ok) {
            breCfgRotation = (int) rotation.u.i;
        }

        toml_datum_t flip = toml_int_in(conf, "flip");
        if (flip.ok) {
            breCfgFlip = (int) flip.u.i;
        }

        toml_datum_t storageLimit = toml_int_in(conf, "storageLimit");
        if(storageLimit.ok) {
            breCfgStorageLimit = (uint64_t) storageLimit.u.i;
        }
    }

    cleanup:
    free(pathbuf);
}

void breGetConfigEntry(int entryId, void *outData) {
    if(entryId == BRE_CFGE_DISP_WIDTH) {
        *((int *)outData) = breCfgWidth;
    } else if(entryId == BRE_CFGE_DISP_HEIGHT) {
        *((int *)outData) = breCfgHeight;
    } else if(entryId == BRE_CFGE_ESN) {
        *((uint32_t *)outData) = breCfgEsn;
    } else if(entryId == BRE_CFGE_IMEI) {
        strncpy((char *)outData, breCfgImei, 16);
    } else if(entryId == BRE_CFGE_HEAP_SIZE) {
        *((uint32_t *)outData) = breCfgHeapSize;
    } else if(entryId == BRE_CFGE_DEBUG_LOG) {
        *((int *)outData) = breCfgDebugLog;
    } else if(entryId == BRE_CFGE_DISP_FRAMES) {
        *((int *)outData) = breCfgFrames;
    } else if(entryId == BRE_CFGE_FORCE_TIME) {
        *((uint64_t *)outData) = breCfgForcedTime;
    } else if(entryId == BRE_CFGE_KEYPAD) {
        *((int *)outData) = breCfgKeypad;
    } else if(entryId == BRE_CFGE_STORAGE_LIMIT) {
        *((uint64_t *)outData) = breCfgStorageLimit;
    } else if (entryId == BRE_CFGE_ROTATION) {
        *((int *)outData) = breCfgRotation;
    } else if (entryId == BRE_CFGE_FLIP) {
        *((int *)outData) = breCfgFlip;
    }
}
