#include <stdlib.h>
#include "../OEMFeatures.h"
#include "md5.h"
#include "AEEComdef.h"
#include "OEMMD5.h"

void OEMMD5_Init(OEMMD5_CTX *context) {
    MD5Init ((MD5_CTX *)context);
}

void OEMMD5_Update(OEMMD5_CTX *context, uint8 *input, uint32 inputLen) {
    MD5Update ((MD5_CTX *)context, input, inputLen);
}

void OEMMD5_Final (uint8 digest[16], OEMMD5_CTX *context) {
    MD5Final (digest, (MD5_CTX *)context);
}

