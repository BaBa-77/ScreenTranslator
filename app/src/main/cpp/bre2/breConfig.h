#pragma once

#define BRE_CFGE_DISP_WIDTH 0
#define BRE_CFGE_DISP_HEIGHT 1
#define BRE_CFGE_ESN 2
#define BRE_CFGE_IMEI 3
#define BRE_CFGE_HEAP_SIZE 4
#define BRE_CFGE_DEBUG_LOG 5
#define BRE_CFGE_DISP_FRAMES 6
#define BRE_CFGE_FORCE_TIME 7
#define BRE_CFGE_KEYPAD 8
#define BRE_CFGE_STORAGE_LIMIT 9
#define BRE_CFGE_ROTATION 10
#define BRE_CFGE_FLIP 11

#ifdef __cplusplus
extern "C" {
#endif

void breInitConfig();
void breGetConfigEntry(int entryId, void *outData);

#ifdef __cplusplus
};
#endif
