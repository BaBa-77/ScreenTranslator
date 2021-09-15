#include "OEMStdLib.h"
#include "OEMRan.h"
#include "OEMMD5.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void OEMRan_Seed(uint32 seed) {
    srand(seed);
    return;
}

uint32 OEMRan_Next(void) {
    return (uint32)rand();
}

void OEMRan_GetNonPseudoRandomBytes(byte *pbRand, int *pcbLen){
    if (pbRand && pcbLen) {
        // clear buffer
        memset(pbRand, 0, *pcbLen);

        // fill with random data from a purely random source
        int fh = open("/dev/random", O_RDONLY | O_NONBLOCK);
        if(fh < 0) {
            return;
        }
        int outSize = read(fh, pbRand, *pcbLen);
        if(outSize >= 0) {
            *pcbLen = outSize;
        }
        close(fh);
    }
}
