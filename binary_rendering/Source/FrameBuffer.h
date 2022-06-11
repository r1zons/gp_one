#pragma once
#include <cstdint>

#define FRAMEBUFFER_WIDTH     uint16_t(512)
#define FRAMEBUFFER_HEIGHT    uint16_t(512)

#define FRAMEBUFFER_TILES_X   uint16_t(FRAMEBUFFER_WIDTH/16)    // 32
#define FRAMEBUFFER_TILES_Y   uint16_t(FRAMEBUFFER_HEIGHT)      // 512

#define FRAMEBUFFER_BUF_SIZE  uint16_t(FRAMEBUFFER_TILES_X*FRAMEBUFFER_TILES_Y) // 32 * 512 = 16384

enum class BackGroundColor {
    BLACK, WHITE
};

struct FrameBuffer {
    uint16_t color[FRAMEBUFFER_BUF_SIZE];

    FrameBuffer();
    FrameBuffer(const FrameBuffer &);
};