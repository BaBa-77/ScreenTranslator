#include "breScreenshot.h"

#include "../bre/OEMDisplayDevImpl.h"
#include "OEMFS.h"
#include "breConfig.h"

#include <AEEIDIB.h>
#include <AEEShell.h>
#include <AEE_OEM.h>
#include <AEEStdLib.h>
#include <stdlib.h>
#include "stb_image_write.h"
#include <limits.h>
#include <time.h>

void breScreenshot() {
    AEEBitmapInfo bmInfo;
    IBitmap_GetInfo(gpDevBitmap, &bmInfo, sizeof(bmInfo));

    unsigned char *rgb = malloc(bmInfo.cx * bmInfo.cy * 3);

    for(int y = 0; y < bmInfo.cy; y++) {
        for(int x = 0; x < bmInfo.cx; x++) {
            NativeColor nc;
            IBitmap_GetPixel(gpDevBitmap, x, y, &nc);
            RGBVAL color = IBitmap_NativeToRGB(gpDevBitmap, nc);
            uint8 r = (color >> 8u) & 0xFFu;
            uint8 g = (color >> 16u) & 0xFFu;
            uint8 b = (color >> 24u) & 0xFFu;

            rgb[y * bmInfo.cx * 3 + x * 3 + 0] = r;
            rgb[y * bmInfo.cx * 3 + x * 3 + 1] = g;
            rgb[y * bmInfo.cx * 3 + x * 3 + 2] = b;
        }
    }

    char buf[256];
    time_t timer = time(NULL);
    struct tm *tm_info = localtime(&timer);
    strftime(buf, 256, "screenshot-%d-%m-%Y_%H-%M-%S.png", tm_info);

    char *path = malloc(PATH_MAX);
    char brewpath[256];
    int brewpathsiz = 256;
    int pathsiz = PATH_MAX;
    MAKEPATH("fs:/", buf, brewpath, &brewpathsiz);
    OEMFS_GetNativePath(brewpath, path, &pathsiz);
    stbi_write_png(path, bmInfo.cx, bmInfo.cy, 3, rgb, bmInfo.cx * 3);
    free(path);
}