#pragma once

#include <string>
struct Scene;

class SceneLoader {
public:
    static Scene loadScene(const std::string &path);
    static void saveScene(const std::string &path, const Scene &scene);
};