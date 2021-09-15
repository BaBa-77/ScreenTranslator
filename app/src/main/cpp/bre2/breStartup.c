#include <AEE_OEM.h>

#include "../brewappmgr/BrewAppMgr.bid"

void StartLauncherApp( void *pUnused ) {
    IShell *pShell = AEE_GetShell();
    if(NULL != pShell) {
        ISHELL_StartApplet(pShell, AEECLSID_BREWAPPMGR_BID);
    }
}