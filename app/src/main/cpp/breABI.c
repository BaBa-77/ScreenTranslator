#include <string.h>
#include <errno.h>

volatile int *__rt_errno_addr(void) {
    return &errno;
}
