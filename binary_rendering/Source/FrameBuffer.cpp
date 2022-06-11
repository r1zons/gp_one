#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{}


FrameBuffer::FrameBuffer(const FrameBuffer &other) {
    for (int i = 0; i < FRAMEBUFFER_BUF_SIZE; ++i) {
        color[i] = other.color[i];
    }
}

