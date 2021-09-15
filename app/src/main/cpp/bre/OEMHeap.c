#include "OEMHeap.h"

#include <sys/mman.h>
#include "../bre2/breConfig.h"

void * g_puHeapBytes   = NULL;
uint32 g_uHeapBytesLen = BRE_CONFIG_HEAP_SIZE;

int OEM_GetHeapInitBytes(void **ppHeapBytes, uint32 *puHeapBytesLen) {
    if(!g_puHeapBytes) {
        g_puHeapBytes = mmap(NULL, g_uHeapBytesLen, PROT_EXEC | PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(g_puHeapBytes == MAP_FAILED) {
            g_puHeapBytes = NULL;
            return EFAILED;
        }
    }

    *ppHeapBytes = g_puHeapBytes;
    *puHeapBytesLen = g_uHeapBytesLen;

   return SUCCESS;
}
