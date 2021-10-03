extern "C" {
#include "OEMFS.h"
#include "AEEStdLib.h"
}

#include "OEMFSPath_priv.h"

#include <climits>
#include <string>
#include "../bre2/brewemu.h"

typedef struct OEMFSGNPMap {
    const char *cpszBREW;
    const char *cpszOEM;
    unsigned    bAutoCreate:1;
} OEMFSGNPMap;

static const OEMFSGNPMap gapmBrewToOEM[] = {
        { AEEFS_RINGERS_DIR, "ringers", TRUE },
        { AEEFS_MOD_DIR, "mod", TRUE },
        { AEEFS_MIF_DIR, "mif", TRUE },
        { AEEFS_ADDRESS_DIR, "address", TRUE },
        { AEEFS_SHARED_DIR, "shared", TRUE },
        { AEEFS_ROOT_DIR, "", TRUE },
        { nullptr, nullptr, 0 },
};

uint16 OEMFS_GetMaxPathLength(void) {
    const OEMFSGNPMap *ppm;
    uint16 uMaxPath;

    for (uMaxPath = 0, ppm = gapmBrewToOEM; ppm->cpszBREW; ppm++) {
        if (!ppm->cpszOEM) {
            continue;
        }

        std::string extFilesPath(gExternalFilesDir);
        std::string fullPath = extFilesPath + '/' + ppm->cpszOEM;
        uint16 uLen = PATH_MAX - (fullPath.size()+1) + strlen(ppm->cpszBREW);

        uMaxPath = MAX(uLen, uMaxPath);
    }

    return uMaxPath;
}

boolean OEMFS_IsAutoCreate(const char *cpszIn)
{
    const OEMFSGNPMap *ppm;

    for (ppm = gapmBrewToOEM; (char *)0 != ppm->cpszBREW; ppm++) {
        const char *cpszTail;

        if ((char *)0 == ppm->cpszOEM) {
            continue;
        }
        std::string extFilesPath(gExternalFilesDir);
        std::string fullPath = extFilesPath + '/' + ppm->cpszOEM;
        cpszTail = SPLITPATH(cpszIn,fullPath.c_str());

        if ((char *)0 != cpszTail && '\0' == cpszTail[0]) {
            return ppm->bAutoCreate;
        }
    }

    return FALSE;
}

int OEMFS_GetNativePath(const char *cpszIn, char *pszOut, int *pnOutLen) {
    int nErr = EBADFILENAME;

    for (const OEMFSGNPMap *ppm = gapmBrewToOEM; ppm->cpszBREW; ppm++) {
        const char *cpszTail = SPLITPATH(cpszIn, ppm->cpszBREW);

        if (cpszTail) {
            if (ppm->cpszOEM) { /* path explicitly unsupported */
                std::string extFilesPath(gExternalFilesDir);
                std::string fullPath = extFilesPath + '/' + ppm->cpszOEM + '/' + cpszTail;

                const char *fullPathStr = fullPath.c_str();

                *pnOutLen = fullPath.size() + 1;

                if (pszOut) {
                    strcpy(pszOut, fullPathStr);

                    nErr = SUCCESS;
                } else {
                    nErr = SUCCESS;
                }
            }
            break;
        }
    }

    return nErr;
}