#include "MMPlayer.h"

#include "MMStream.h"

MMPlayer::MMPlayer() : m_stream(nullptr), mmf(MMF_NONE) {

}

MMPlayer::~MMPlayer() {
    delete m_stream;
}

void MMPlayer::setMediaSource(void *data, uint32_t size) {
    if(!m_stream) {
        delete m_stream;
    }
    m_stream = new MMByteStream(data, size);
}
