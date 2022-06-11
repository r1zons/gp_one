#pragma once

#include "Sprite.h"
#include "FrameBuffer.h"
#include <string>

class ImageManager {
public:
    ImageManager() = delete;
    static Sprite loadSprite(const std::string &path);
    static void saveSprite(const std::string &path, const Sprite &sprite);
    static FrameBuffer loadFrameBuffer(const std::string &path);
    static void saveFrameBuffer(const std::string &path, const FrameBuffer &frameBuffer);
};