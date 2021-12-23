#include "MMStream.h"

#include <cstring>

MMByteStream::MMByteStream(void *data, size_t size) :
        m_data((unsigned char *) data),
        m_size(size),
        m_currentOffset(0) {}

uint32_t MMByteStream::read(uint32_t readSize, void *readData) {
    if (readSize > m_size - m_currentOffset) {
        readSize = m_size - m_currentOffset;
    }
    memcpy(readData, m_data + m_currentOffset, readSize);
    m_currentOffset += readSize;
    return readSize;
}
