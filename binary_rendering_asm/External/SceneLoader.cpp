#include "SceneLoader.h"

#include "Scene.h"
#include "ImageManager.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>


Scene SceneLoader::loadScene(const std::string &path) {
    Scene scene;
    std::ifstream f(path);
    if (!f) {
        throw std::runtime_error("Failed to open file " + path);
    }
    std::string line;
    std::getline(f, line);
    if (line == "WHITE") {
        scene.bkgColor = BackGroundColor::WHITE;
    } else if (line == "BLACK") {
        scene.bkgColor = BackGroundColor::BLACK;
    } else {
        throw std::runtime_error("unsupported background color: " + line);
    }

    std::unordered_map<std::string, uint16_t> nameToInd;

    for (; std::getline(f, line); ) {
        SpriteInstance sprite;
        std::string name;
        std::stringstream ss(line);
        ss >> sprite.x;
        ss.ignore();
        ss >> sprite.y;
        ss.ignore();
        ss >> name;

        if (nameToInd.find(name) == nameToInd.end()) {
            int spriteCount = nameToInd.size();
            nameToInd[name] = spriteCount;
            scene.spriteNames.push_back(name);
            scene.sprites.push_back(ImageManager::loadSprite(name));
        }
        sprite.ind = nameToInd[name];
        scene.spriteInstances.push_back(sprite);
    }

    return scene;
}

void SceneLoader::saveScene(const std::string &path, const Scene &scene) {
    std::ofstream f(path);
    if (!f) {
        throw std::runtime_error("Failed to open file " + path);
    }

    if (scene.bkgColor == BackGroundColor::WHITE) {
        f << "WHITE" << std::endl;
    } else if (scene.bkgColor == BackGroundColor::BLACK) {
        f << "BLACK" << std::endl;
    }

    for (int i = 0; i < scene.spriteInstances.size(); ++i) {
        auto & sprite = scene.spriteInstances[i];
        f << sprite.x <<","<< sprite.y <<","<<scene.spriteNames[sprite.ind]<<std::endl;
    }
}
