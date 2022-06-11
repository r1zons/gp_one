// Версия с AVX
// Пока не начали реализовывать сразу хочется отметить, что даже использование avx-512 не повлияет на производительность в
// лучшую сторону значительно потому что ускорить функции загрузки/выгрузки векторные инструкции не смогут (очевидно из используемых
// функций), следовательно, попробуем ускорить отрисовку спрайта. Тут снова проблема, по условию задачи у нас строки по 512, это означает, что даже
// на 512 битных регистрах мы можем отрисовать не более одной строки одного спрайта за один раз (Очевидно из Дирихле, можно
// также показать это на картинке). Это говорит о том, что само по себе использование векторных инструкций в коде не очень
// востребованно в данной задаче. Можно попытаться подсчёт снизить за счёт avx, однако это гиблое дело, потому что
// нам всё равно приходится огромное кол-во раз обращаться к памяти
// Нашей задаче больше влияет обращение к памяти, чем арифметические операции
// то есть я не смогу сэкономить достаточно времени на подсчёте, минуя обращение к памяти потому что обращение более
// дорогая операция, чем даже не очень простой подсчёт.
// Использование в данной задаче avx, всё равно что стрелять из пушки по воробьям. Мало того, что сложно в реализации и
// имеет под собой много проблем, так ещё и прироста не даёт фактического.

// Ближе к приколам реализации
//
// Сделал union для uint64_t так можно гораздо быстрее обращаться к нужному части элемента так как переписывать остальное
// под uint64_t из uint16_t (тесты, структуры и остальное) мне лень

// Задание считаю не очень полезным, так как нам предложили поковырять лопатой в песочнице. Использование 128+ битных
// векторных инструкций было бы обусловлено, если бы ширина/высота спрайта была минимум 128.
// По фак

#include <iostream>
#include <cstring>
#include <immintrin.h>
#include "GP_ONE.h"
#include <cassert>

Sprite   GP_ONE::spriteMemory[MAX_SPRITE_COUNT];
uint16_t GP_ONE::frameBuffer[FRAMEBUFFER_BUF_SIZE];


uint16_t x, y, xMod16, xDiv16,
        cordInGrid,
        spriteColor, alpha,
        leftColor, leftAlpha, rightColor, rightAlpha,
        inst, ySpriteTileCord, xSpriteTileCord, indexInSprite;


void GP_ONE::loadSprites(const Sprite *sprites, uint16_t spriteCount) {
    memcpy(spriteMemory, sprites, spriteCount << 10);
}


void GP_ONE::clearFrameBuffer(BackGroundColor bkgColor) {
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


union ui64 {
    uint64_t one;
    uint16_t four[4];
};


ui64 countNewColor(ui64 _frame, ui64 _sprite, ui64 _alpha) {
//    return _frame ^ (_sprite & _alpha) ^ (_frame & _alpha);
    return ui64{_frame.one & (~(_alpha.one)) | _sprite.one & _alpha.one};
}


uint64_t buildNewLL(uint16_t a, uint16_t b, uint16_t c, uint16_t d) {
    return ((uint64_t)a << 48) + ((uint64_t)b << 32) + ((uint64_t)c << 16) + ((uint64_t)d);
}



uint64_t buildFrameLL(uint16_t cordInGrid) {
#ifndef NDEBUG
    if (cordInGrid + 3 >= FRAMEBUFFER_BUF_SIZE) {
        std::cerr << cordInGrid / 32 << " " << cordInGrid % 32 << "\n";
        exit(-1);
    }
#endif
    return buildNewLL(GP_ONE::frameBuffer[cordInGrid], GP_ONE::frameBuffer[cordInGrid + 1],
                      GP_ONE::frameBuffer[cordInGrid + 2], GP_ONE::frameBuffer[cordInGrid + 3]);
}


void GP_ONE::drawSpriteInstances(const SpriteInstance *instances, uint16_t instanceCount) {
    for (inst = 0; inst < instanceCount; ++inst) {
//    int to = 0;
//    for (inst = to; inst <= to + 1; inst++) {
        x = instances[inst].x, y = instances[inst].y;
        xMod16 = x & 0x000F, xDiv16 = x >> 4;
        indexInSprite = instances[inst].ind;

        if (!xMod16) {
//             frame - a, color - b, alpha - c
//             a(~c) + bc
            for (ySpriteTileCord = 0; ySpriteTileCord < SPRITE_TILES_Y; ySpriteTileCord += 4) {
                cordInGrid = ((y + ySpriteTileCord) << 5) + xDiv16;
//
//                // берём по 4 строки
//                // выполняем формулку
//                // возвращаем значения
/*
                   [][][][]
                   [][][][]
                   [][][][]
                   [][][][]

                   [][][][]
                   [][][][]
                   [][][][]
                   [][][][]

                   [][][][]
                   ...
*/
                __m256i _color = _mm256_loadu_si256((const __m256i *)&(spriteMemory[indexInSprite].color[ySpriteTileCord << 2]));
                __m256i _alpha = _mm256_loadu_si256((const __m256i *)&(spriteMemory[indexInSprite].alpha[ySpriteTileCord << 2]));
                __m256i _frame = _mm256_setr_epi16(
                        frameBuffer[cordInGrid + 96], frameBuffer[cordInGrid + 1 + 96], frameBuffer[cordInGrid + 2 + 96], frameBuffer[cordInGrid + 3 + 96],
                        frameBuffer[cordInGrid + 64], frameBuffer[cordInGrid + 1 + 64], frameBuffer[cordInGrid + 2 + 64], frameBuffer[cordInGrid + 3 + 64],
                        frameBuffer[cordInGrid + 32], frameBuffer[cordInGrid + 1 + 32], frameBuffer[cordInGrid + 2 + 32], frameBuffer[cordInGrid + 3 + 32],
                        frameBuffer[cordInGrid], frameBuffer[cordInGrid + 1], frameBuffer[cordInGrid + 2], frameBuffer[cordInGrid + 4]
                );

                __m256i _helper1, _helper2;

                _helper1 = _mm256_and_si256(_frame, _alpha);
                _helper2 = _mm256_and_si256(_color, _alpha);
                _helper1 = _mm256_xor_si256(_helper1, _helper2);
                _frame = _mm256_xor_si256(_frame, _helper1);
//
//                _helper1 = _mm256_and_si256(_color, _alpha);
//                _helper2 = _mm256_andnot_si256(_alpha, _frame);
//                _frame = _mm256_or_si256(_helper1, _helper2);

                uint16_t output[16];

                _mm256_storeu_si256((__m256i *)&output[0], _frame);

                for (int i = 0; i < 4; ++i) {
                    frameBuffer[cordInGrid + (i<<5)] = output[i<<2];
                    frameBuffer[cordInGrid + (i<<5) + 1] = output[(i<<2) + 1];
                    frameBuffer[cordInGrid + (i<<5) + 2] = output[(i<<2) + 2];
                    frameBuffer[cordInGrid + (i<<5) + 3] = output[(i<<2) + 3];
                }

            }
        } else {
            for (ySpriteTileCord = 0; ySpriteTileCord < SPRITE_TILES_Y; ++ySpriteTileCord) {
                cordInGrid = ((y + ySpriteTileCord) << 5) + xDiv16;

                uint16_t spriteColor1 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2)];
                uint16_t spriteColor2 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 1];
                uint16_t spriteColor3 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 2];
                uint16_t spriteColor4 = spriteMemory[indexInSprite].color[(ySpriteTileCord << 2) + 3];

                uint16_t alpha1 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2)];
                uint16_t alpha2 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 1];
                uint16_t alpha3 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 2];
                uint16_t alpha4 = spriteMemory[indexInSprite].alpha[(ySpriteTileCord << 2) + 3];

                ui64 _sprite = {buildNewLL(spriteColor1, spriteColor2, spriteColor3, spriteColor4)};

                ui64 _alpha = {buildNewLL(alpha1, alpha2, alpha3, alpha4)};

                ui64 _frame = {buildNewLL(frameBuffer[cordInGrid], frameBuffer[cordInGrid + 1],
                                          frameBuffer[cordInGrid + 2], frameBuffer[cordInGrid + 3])};

                ui64 _alphaLeft = {_alpha.one >> xMod16};
                ui64 _spriteLeft = {_sprite.one >> xMod16};
                _frame = countNewColor(_frame, _spriteLeft, _alphaLeft);

                uint16_t _color5 = spriteColor4 << (16 - xMod16);
                uint16_t _alpha5 = alpha4 << (16 - xMod16);

                frameBuffer[cordInGrid] = _frame.four[3];
                frameBuffer[cordInGrid + 1] = _frame.four[2];
                frameBuffer[cordInGrid + 2] = _frame.four[1];
                frameBuffer[cordInGrid + 3] = _frame.four[0];

                frameBuffer[cordInGrid + 4] ^= (_color5 & _alpha5) ^ (frameBuffer[cordInGrid + 4] & _alpha5);
            }
        }
    }
}


void GP_ONE::saveFrameBuffer(FrameBuffer &outFrameBuffer) {
    memcpy(&outFrameBuffer, frameBuffer, FRAMEBUFFER_BUF_SIZE << 1);
}