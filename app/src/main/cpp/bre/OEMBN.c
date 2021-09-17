#include "AEEStdLib.h"
#include "AEE_OEM.h"
#include "AEE_OEMDispatch.h"

#include "OEMBN.h"

boolean OEMBN_Supported(void) {
    return FALSE;
}

void OEMBN_ModExp ( const uint8       *pbMsgBuf,
                    uint16       uMsgLen,
                    const uint8       *pbExponentBuf,
                    uint16       uExponentLen,
                    const uint8       *pbModulusBuf,
                    uint16       uModulusLen,
                    uint8       *pResultBuf,
                    uint32      *puResultLen,
                    int         *pnError,
                    AEECallback *pcb )
{
    ACONTEXT           *pac;
    *pnError = EUNSUPPORTED;
    if (pcb) {
        pac = AEE_GetAppContext();
        AEE_ResumeCallback((AEECallback *)pcb, pac);
    }
}

