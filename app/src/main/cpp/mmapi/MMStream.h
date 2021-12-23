#pragma once

#include <cstdint>

class MMStream {
public:
    virtual ~MMStream() = default;

    virtual uint32_t read(uint32_t size, void *data) = 0;
};

class MMByteStream : public MMStream {
    unsigned char *m_data;
    size_t m_size;
    size_t m_currentOffset;
public:
    MMByteStream(void *data, size_t size);

    uint32_t read(uint32_t readSize, void *readData) override;
};
