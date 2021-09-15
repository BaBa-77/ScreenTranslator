#if !defined(OEMFSPATH_PRIV_H)
#define OEMFSPATH_PRIV_H

#if defined(__cplusplus)
extern "C" {
#endif

// int     OEMFS_GetBREWPath(const char *cpszIn, char *pszOut, int *pnOutLen);
boolean OEMFS_IsAutoCreate(const char *cpszIn);

#if defined(__cplusplus)
}
#endif

#endif