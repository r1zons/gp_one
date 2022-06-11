// В отличии от версии на 16-битной архитектуре СССР здесь я использовал 64-битный ассемблер
// В проекте хранятся и другие ассемблерные файлы, которые использовались для отладки и проверки
// корректноти работы
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Обращаю ваше внимание, что данная версия работает только на Linix/Unix так как команды ассемблера
// на Windows другие
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <string.h>
#include <iostream>
#include "GP_ONE.h"


Sprite   GP_ONE::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE::frameBuffer[FRAMEBUFFER_BUF_SIZE];

uint16_t x, y, xMod16, xDiv16, revXMod16,
        cordInGrid,
        spriteColor, alpha, 
        leftColor, leftAlpha, rightColor, rightAlpha,
        inst, ySpriteTileCord, xSpriteTileCord, indexInSprite;



void GP_ONE::loadSprites(const Sprite *sprites, uint16_t spriteCount) {
    // for (uint16_t i = 0; i < spriteCount; ++i) {
    //     spriteMemory[i] = sprites[i];
    // }

    memcpy(spriteMemory, sprites, spriteCount << 10);
}


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



// Во время прорисовки спрайти в начале сетки можно использовать ассемблер, это экономит нам время,
// но реализация не из приятных из-за методов хранения чисел в данных компьютера
void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {

    for (inst = 0; inst < instanceCount; ++inst) {               
        x = instances[inst].x, y = instances[inst].y;
        xMod16 = x & 0x000F, xDiv16 = x >> 4, revXMod16 = 16 - xMod16;
        indexInSprite = instances[inst].ind;
        cordInGrid = (y << 5) + xDiv16;
        for (ySpriteTileCord = 0; ySpriteTileCord < SPRITE_TILES_Y; ++ySpriteTileCord, cordInGrid += 32) {
            


            if (!xMod16) {         
                load_of_0(frameBuffer, cordInGrid, &spriteMemory[indexInSprite], ySpriteTileCord, xMod16);

                //frameBuffer[cordInGrid] ^= (spriteColor1 & alpha1) ^ (frameBuffer[cordInGrid] & alpha1);
                //frameBuffer[cordInGrid + 1] ^= (spriteColor2 & alpha2) ^ (frameBuffer[cordInGrid + 1] & alpha2);
                //frameBuffer[cordInGrid + 2] ^= (spriteColor3 & alpha3) ^ (frameBuffer[cordInGrid + 2] & alpha3);
                //frameBuffer[cordInGrid + 3] ^= (spriteColor4 & alpha4) ^ (frameBuffer[cordInGrid + 3] & alpha4);

            } else {



                uint16_t spriteColor[4];
                uint16_t alpha[4];

                spriteColor[0] = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2)];
                alpha[0] = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2)];
                
                spriteColor[1] = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 1];
                alpha[1] = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 1];
                
                spriteColor[2] = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 2];
                alpha[2] = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 2];
                
                spriteColor[3] = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 3];
                alpha[3] = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 3];

                uint16_t _color[5];
                uint16_t _alpha[5];


                _color[0] = spriteColor[0] >> xMod16;
                _alpha[0] = alpha[0] >> xMod16;
                _color[1] = spriteColor[1] >> xMod16 | spriteColor[0] << revXMod16;
                _alpha[1] = alpha[1] >> xMod16 | alpha[0] << revXMod16;
                _color[2] = spriteColor[2] >> xMod16 | spriteColor[1] << revXMod16;
                _alpha[2] = alpha[2] >> xMod16 | alpha[1] << revXMod16;
                _color[3] = spriteColor[3] >> xMod16 | spriteColor[2] << revXMod16;
                _alpha[3] = alpha[3] >> xMod16 | alpha[2] << revXMod16;
                _color[4] = spriteColor[3] << revXMod16;
                _alpha[4] = alpha[3] << revXMod16;


                frameBuffer[cordInGrid]     ^= (_color[0] & _alpha[0]) ^ (frameBuffer[cordInGrid    ] & _alpha[0]);
                frameBuffer[cordInGrid + 1] ^= (_color[1] & _alpha[1]) ^ (frameBuffer[cordInGrid + 1] & _alpha[1]);
                frameBuffer[cordInGrid + 2] ^= (_color[2] & _alpha[2]) ^ (frameBuffer[cordInGrid + 2] & _alpha[2]);
                frameBuffer[cordInGrid + 3] ^= (_color[3] & _alpha[3]) ^ (frameBuffer[cordInGrid + 3] & _alpha[3]);
                frameBuffer[cordInGrid + 4] ^= (_color[4] & _alpha[4]) ^ (frameBuffer[cordInGrid + 4] & _alpha[4]);

            }
        }
    }
}

void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    memcpy(&outFrameBuffer, frameBuffer, FRAMEBUFFER_BUF_SIZE << 1);
    // for (uint16_t i = 0; i < FRAMEBUFFER_BUF_SIZE; ++i) {
        // outFrameBuffer.color[i] = frameBuffer[i];
    // }   
}
