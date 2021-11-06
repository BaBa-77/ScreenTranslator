#pragma once

#define BRE_CFGE_DISP_WIDTH 0
#define BRE_CFGE_DISP_HEIGHT 1
#define BRE_CFGE_ESN 2
#define BRE_CFGE_IMEI 3
#define BRE_CFGE_HEAP_SIZE 4
#define BRE_CFGE_DEBUG_LOG 5
#define BRE_CFGE_DISP_FRAMES 6
#define BRE_CFGE_FORCE_TIME 7

#ifdef __cplusplus
extern "C" {
#endif

void breInitConfig();
void breGetConfigEntry(int entryId, void *outData);

#ifdef __cplusplus
};
#endif

#define BRE_CONFIG_ESN 0xab2b3c4f
#define BRE_CONFIG_IMEI "1234567812345678"
#define BRE_CONFIG_HEAP_SIZE (64 * 1024 * 1024)
#define BRE_ENABLE_PLATFORM_DEBUG
#define BRE_DISPLAY_CONFIG_FRAMES 0

// GPS timestamp; set to zero if you want to use system time
// #define BRE_FORCED_TIME 996877357ULL
#define BRE_FORCED_TIME 0
