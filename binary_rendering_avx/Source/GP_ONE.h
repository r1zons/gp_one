#pragma once

#include "Sprite.h"
#include "SpriteInstance.h"
#include "FrameBuffer.h"

#define MAX_SPRITE_COUNT 32

class GP_ONE {
public:
    static Sprite   spriteMemory[MAX_SPRITE_COUNT];
    static uint16_t frameBuffer[FRAMEBUFFER_BUF_SIZE];

    GP_ONE() = delete;

    static void loadSprites(const Sprite *sprites, uint16_t spriteCount);
    static void clearFrameBuffer(BackGroundColor bkgColor);
    static void drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount);
    static void saveFrameBuffer(FrameBuffer &outFrameBuffer);
};
