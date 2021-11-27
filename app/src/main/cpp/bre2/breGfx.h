#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ANativeWindow;

void breGfxAcqCx();
void breGfxRelCx();
void breGfxSwap();
int breGfxIsInitialized();

extern struct ANativeWindow *gNativeWindow;

#ifdef __cplusplus
}
#endif
