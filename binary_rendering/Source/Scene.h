#pragma once

#include "Sprite.h"
#include "FrameBuffer.h"
#include "SpriteInstance.h"
#include <string>
#include <vector>


struct Scene {
    BackGroundColor              bkgColor = BackGroundColor::BLACK;
    std::vector<Sprite>          sprites;
    std::vector<std::string>     spriteNames;
    std::vector<SpriteInstance>  spriteInstances;
};