#pragma once
#include <core/graphics/mesh.h>
#include <string>
#include <unordered_map>

namespace ResourceManager
{
    extern std::unordered_map<std::string, Graphics::Texture> textures;
    extern std::unordered_map<std::string, Graphics::Mesh> meshes;

    void init();
    void shutdown();

    std::string getResourceFolderPath();

    Graphics::Texture loadTextureFromFile(std::string filepath);
}