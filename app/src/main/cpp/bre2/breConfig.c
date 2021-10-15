#include "breConfig.h"

#include "OEMFS.h"
#include <stdlib.h>
#include <limits.h>
#include "toml.h"

static int breCfgWidth = 240;
static int breCfgHeight = 320;

void breInitConfig() {
    char *pathbuf = malloc(PATH_MAX);
    int pathbufSize = PATH_MAX;
    if(SUCCESS == OEMFS_GetNativePath("fs:/sys/breConfig.toml", pathbuf, &pathbufSize)) {
        FILE *fp = fopen(pathbuf, "r");
        if(!fp) {
            goto cleanup;
        }

        char errbuf[200];
        toml_table_t *conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
        fclose(fp);

        if(!conf) {
            goto cleanup;
        }

        toml_datum_t width = toml_int_in(conf, "width");
        if(width.ok) {
            breCfgWidth = (int) width.u.i;
        }

        toml_datum_t height = toml_int_in(conf, "height");
        if(height.ok) {
            breCfgHeight = (int) height.u.i;
        }
    }

    cleanup:
    free(pathbuf);
}

void breGetConfigEntry(int entryId, void *outData) {
    if(entryId == BRE_CFGE_DISP_WIDTH) {
        *((int *)outData) = breCfgWidth;
    } else if(entryId == BRE_CFGE_DISP_HEIGHT) {
        *((int *)outData) = breCfgHeight;
    }
}
