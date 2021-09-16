#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ANativeWindow;

void breGfxInit(struct ANativeWindow *win);
void breGfxDestroy();
void breGfxAcqCx();
void breGfxRelCx();
void breGfxSwap();
extern struct ANativeWindow *gNativeWindow;

#ifdef __cplusplus
}
#endif
