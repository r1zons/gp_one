#include "ImageManager.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

static uint16_t encodeTile(const stbi_uc* pixels,
                    int width, int channels,
                    int i, int tile_j, int c
) {
    uint16_t tile = 0;

    for (int local_j = 0; local_j < 16; ++local_j) {
        int j = local_j + tile_j*16;
        uint16_t bit = pixels[i * (width*channels) + j * channels + c] / 255;
        tile = (tile << 1) | bit;
    }

    return tile;
}

static void decodeTile(uint16_t tile, stbi_uc* pixels,
                           int width, int channels,
                           int i, int tile_j, int c
) {
    for (int local_j = 15; local_j >= 0; --local_j) {
        int j = local_j + tile_j*16;
        pixels[i * (width*channels) + j * channels + c] = (tile & 1) * 255;
        tile = tile >> 1;
    }
}

static void saveImage(const std::string &path, stbi_uc *pixels, int width, int height, int channels) {
    int len = path.size();
    if (strcmp(&(path.data()[len - 3]), "png") == 0) {
        stbi_write_png(path.data(), width, height, channels, pixels, 0);
    } else if (strcmp(&(path.data()[len - 3]), "bmp") == 0) {
        stbi_write_bmp(path.data(), width, height, channels, pixels);
    } else if (strcmp(&(path.data()[len - 3]), "tga") == 0) {
        stbi_write_tga(path.data(), width, height, channels, pixels);
    } else if (strcmp(&(path.data()[len - 3]), "jpg") == 0) {
        stbi_write_jpg(path.data(), width, height, channels, pixels, 100);
    } else {
        std::stringstream ss;
        ss << "Image " <<path<<" has unsupported extension";
        throw std::runtime_error(ss.str());
    }
}

Sprite ImageManager::loadSprite(const std::string &path) {
    int width, height, channels;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, 2);
    if (!pixels) {
        std::stringstream ss;
        ss << "Failed to load image on path: " << path;
        throw std::runtime_error(ss.str());
    }
    if ((width != SPRITE_TILES_X*16) || (height != SPRITE_TILES_Y)) {
        std::stringstream ss;
        ss << "Image " <<path<<" has incorrect resolution"<<width<<"x"<<height;
        throw std::runtime_error(ss.str());
    }
    if (channels != 2) {
        std::stringstream ss;
        ss << "Image " <<path<<" has incorrect channel count = "<<channels;
        throw std::runtime_error(ss.str());
    }

    Sprite outSprite;

    for (int i = 0; i < SPRITE_TILES_Y; ++i) {
        for (int j = 0; j < SPRITE_TILES_X; ++j) {
            outSprite.color[i*SPRITE_TILES_X + j] = encodeTile(pixels, width, channels, i, j, 0);
            outSprite.alpha[i*SPRITE_TILES_X + j] = encodeTile(pixels, width, channels, i, j, 1);
        }
    }

    return outSprite;
}

FrameBuffer ImageManager::loadFrameBuffer(const std::string &path) {
    int width, height, channels;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, 1);
    if (!pixels) {
        std::stringstream ss;
        ss << "Failed to load image on path: " << path;
        throw std::runtime_error(ss.str());
    }
    if ((width != FRAMEBUFFER_TILES_X*16) || (height != FRAMEBUFFER_TILES_Y)) {
        std::stringstream ss;
        ss <<"Image "<<path<<" has incorrect resolution"<<width<<"x"<<height;
        throw std::runtime_error(ss.str());
    }
    if (channels != 1) {
        std::stringstream ss;
        ss << "Image " <<path<<" has incorrect channel count = "<<channels;
        throw std::runtime_error(ss.str());
    }

    FrameBuffer outFrameBuffer;

    for (int i = 0; i < FRAMEBUFFER_TILES_Y; ++i) {
        for (int j = 0; j < FRAMEBUFFER_TILES_X; ++j) {
            outFrameBuffer.color[i*FRAMEBUFFER_TILES_X + j] = encodeTile(pixels, width, channels, i, j, 0);
        }
    }

    return outFrameBuffer;
}

void ImageManager::saveSprite(const std::string &path, const Sprite &sprite) {
    int width = SPRITE_TILES_X*16;
    int height = SPRITE_TILES_Y;
    int channels = 2;

    auto *pixels = new stbi_uc[width*height*channels];

    for (int i = 0; i < SPRITE_TILES_Y; ++i) {
        for (int j = 0; j < SPRITE_TILES_X; ++j) {
            decodeTile(sprite.color[i*SPRITE_TILES_X + j], pixels, width, channels, i, j, 0);
            decodeTile(sprite.alpha[i*SPRITE_TILES_X + j], pixels, width, channels, i, j, 1);
        }
    }

    saveImage(path, pixels, width, height, channels);
}

void ImageManager::saveFrameBuffer(const std::string &path, const FrameBuffer &frameBuffer) {
    int width = FRAMEBUFFER_TILES_X*16;
    int height = FRAMEBUFFER_TILES_Y;

    auto *pixels = new stbi_uc[width*height];

    for (int i = 0; i < FRAMEBUFFER_TILES_Y; ++i) {
        for (int j = 0; j < FRAMEBUFFER_TILES_X; ++j) {
            decodeTile(frameBuffer.color[i*FRAMEBUFFER_TILES_X + j], pixels, width, 1, i, j, 0);
        }
    }

    saveImage(path, pixels, width, height, 1);
}

