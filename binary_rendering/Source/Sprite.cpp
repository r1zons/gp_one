#include "Sprite.h"

Sprite::Sprite()
{}

Sprite::Sprite(const Sprite &other) {
    for (int i = 0; i < SPRITE_BUF_SIZE; ++i) {
        color[i] = other.color[i];
        alpha[i] = other.alpha[i];
    }
}
