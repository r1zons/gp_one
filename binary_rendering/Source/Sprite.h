#pragma once
#include <cstdint>

#define SPRITE_WIDTH     uint16_t(64)
#define SPRITE_HEIGHT    uint16_t(64)

#define SPRITE_TILES_X   uint16_t(SPRITE_WIDTH/16)  // 4
#define SPRITE_TILES_Y   uint16_t(SPRITE_HEIGHT)    // 64

#define SPRITE_BUF_SIZE  uint16_t(SPRITE_TILES_X*SPRITE_TILES_Y) // 4 * 64

struct Sprite {
    uint16_t color[SPRITE_BUF_SIZE];
    uint16_t alpha[SPRITE_BUF_SIZE];

    Sprite();
    Sprite(const Sprite &);
};


// [....] [....] [....] [....] x = 6

// x / 4 = 1
// x % 4 = 2