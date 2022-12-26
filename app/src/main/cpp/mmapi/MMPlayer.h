#pragma once

#include <stdint.h>

class MMStream;

class MMPlayer {
    MMStream *m_stream;
public:
    enum MMFormat : uint16_t {
        MMF_NONE = 0,
        MMF_DETECT,
        MMF_CMIDI,
    };

    MMPlayer();
    ~MMPlayer();

    void setMediaFormat(MMFormat fmt) {
        mmf = fmt;
    }
    void setMediaSource(void *data, uint32_t size);
private:
    MMFormat mmf;
};
