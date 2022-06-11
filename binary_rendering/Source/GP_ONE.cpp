#include <string.h>

#include "GP_ONE.h"

Sprite   GP_ONE::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE::frameBuffer[FRAMEBUFFER_BUF_SIZE];

// выносим переменные из циклов, чтобы они не создавались каждый раз
uint16_t x, y, xMod16, xDiv16,
        cordInGrid,
        spriteColor, alpha, 
        leftColor, leftAlpha, rightColor, rightAlpha,
        inst, ySpriteTileCord, xSpriteTileCord, indexInSprite;

// вместо обычного заполнения массива будем заполнять память напрямую - нет лишних обращений
// к памяти + это даже быстрее чем векторное потому что мы сразу копируем весь объём
void GP_ONE::loadSprites(const Sprite *sprites, uint16_t spriteCount) {
    // for (uint16_t i = 0; i < spriteCount; ++i) {
    //     spriteMemory[i] = sprites[i];
    // }

    memcpy(spriteMemory, sprites, spriteCount << 10);
}

// аналогично loadSprites
void GP_ONE::clearFrameBuffer(BackGroundColor bkgColor) {
    // for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; ++i) {
    //     frameBuffer[i] = (uint16_t)bkgColor ? 0xFFFF : 0x0000;
    // }

    memset(frameBuffer, (uint16_t)bkgColor ? 0xFFFF : 0x0000, FRAMEBUFFER_BUF_SIZE << 1);
}

/*
    000 0   
    001 0
    010 0
    011 1
    100 1
    101 0
    110 1
    111 1
*/

// берём очередной спрайт
// ищем для него позицию в массиве frameBuffer
// вместо двойного цикла для каждой из осей сделаем один по строкам, а остальное посчитаем
// руками, это ускорит прогамму, но появится дублирование кода. Поскольку производительность для
// нас более выгодный параметр, то оставим так.
// Так же заметим, что для прорисовки спрайта внутри сетки фрейма можно пользоваться одной командой,
// а не множеством. Это экономит нам в среднем достаточное кол-во операций
void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {
    for (inst = 0; inst < instanceCount; ++inst) {               
        x = instances[inst].x, y = instances[inst].y;
        xMod16 = x & 0x000F, xDiv16 = x >> 4;
        indexInSprite = instances[inst].ind;    

        for (ySpriteTileCord = 0; ySpriteTileCord < SPRITE_TILES_Y; ++ySpriteTileCord) {
            cordInGrid = ((y + ySpriteTileCord) << 5) + xDiv16;

            uint16_t spriteColor1 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2)];
            uint16_t alpha1 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2)];
            
            uint16_t spriteColor2 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 1];
            uint16_t alpha2 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 1];
            
            uint16_t spriteColor3 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 2];
            uint16_t alpha3 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 2];
            
            uint16_t spriteColor4 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 3];
            uint16_t alpha4 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 3];



            if (!xMod16) {
                // frame - a, color - b, alpha - c
                // a(~c) + bc

                frameBuffer[cordInGrid] ^= (spriteColor1 & alpha1) ^ (frameBuffer[cordInGrid] & alpha1);
                frameBuffer[cordInGrid + 1] ^= (spriteColor2 & alpha2) ^ (frameBuffer[cordInGrid + 1] & alpha2);
                frameBuffer[cordInGrid + 2] ^= (spriteColor3 & alpha3) ^ (frameBuffer[cordInGrid + 2] & alpha3);
                frameBuffer[cordInGrid + 3] ^= (spriteColor4 & alpha4) ^ (frameBuffer[cordInGrid + 3] & alpha4);


            } else {
                uint16_t _color1 = spriteColor1 >> xMod16;
                uint16_t _alpha1 = alpha1 >> xMod16;
                uint16_t _color2 = spriteColor2 >> xMod16 | spriteColor1 << (16 - xMod16) ;
                uint16_t _alpha2 = alpha2 >> xMod16 | alpha1 << (16 - xMod16);
                uint16_t _color3 = spriteColor3 >> xMod16 | spriteColor2 << (16 - xMod16) ;
                uint16_t _alpha3 = alpha3 >> xMod16 | alpha2 << (16 - xMod16);
                uint16_t _color4 = spriteColor4 >> xMod16 | spriteColor3 << (16 - xMod16) ;
                uint16_t _alpha4 = alpha4 >> xMod16 | alpha3 << (16 - xMod16);
                uint16_t _color5 = spriteColor4 << (16 - xMod16);
                uint16_t _alpha5 = alpha4 << (16 - xMod16);

                frameBuffer[cordInGrid]     ^= (_color1 & _alpha1) ^ (frameBuffer[cordInGrid] & _alpha1);
                frameBuffer[cordInGrid + 1] ^= (_color2 & _alpha2) ^ (frameBuffer[cordInGrid + 1] & _alpha2);
                frameBuffer[cordInGrid + 2] ^= (_color3 & _alpha3) ^ (frameBuffer[cordInGrid + 2] & _alpha3);
                frameBuffer[cordInGrid + 3] ^= (_color4 & _alpha4) ^ (frameBuffer[cordInGrid + 3] & _alpha4);
                frameBuffer[cordInGrid + 4] ^= (_color5 & _alpha5) ^ (frameBuffer[cordInGrid + 4] & _alpha5);

            }
        }
    }
}

// то же самое, что и с первыми двумя функциями
void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    memcpy(&outFrameBuffer, frameBuffer, FRAMEBUFFER_BUF_SIZE << 1);
    // for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; ++i) {
        // outFrameBuffer.color[i] = frameBuffer[i];
    // }   
}
