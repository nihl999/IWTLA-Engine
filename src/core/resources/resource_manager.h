#pragma once
#include <core/graphics/mesh.h>
#include <core/renderer/shader.h>
#include <core/renderer/renderer.h>
#include <string>
#include <unordered_map>

class ResourceManager
{
private:
    static ResourceManager *instance;

public:
    std::unordered_map<std::string, Texture> textures = std::unordered_map<std::string, Texture>();
    std::unordered_map<std::string, Mesh> meshes = std::unordered_map<std::string, Mesh>();

private:
    ResourceManager();

public:
    static ResourceManager &GetInstance();
    ~ResourceManager();

    std::string GetResourceFolderPath();
    Texture LoadTextureFromFile(std::string filepath);
    ShaderProgram LoadShadersFromFile(std::string vertexFile, std::string fragmentFile);
};