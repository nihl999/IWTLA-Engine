#pragma once
#include <core/graphics/mesh.h>
#include <core/graphics/renderer/shader.h>
#include <core/graphics/renderer/renderer.h>
#include <string>
#include <unordered_map>
#include <optional>

class ResourceManager
{
private:
    static ResourceManager *instance;

public:
    std::unordered_map<std::string, Texture> textures = std::unordered_map<std::string, Texture>();
    std::unordered_map<std::string, Mesh> meshes = std::unordered_map<std::string, Mesh>();
    std::unordered_map<std::string, ShaderProgram> shaders = std::unordered_map<std::string, ShaderProgram>();

private:
    ResourceManager();

public:
    static ResourceManager &GetInstance();
    ~ResourceManager();

    std::optional<Texture> GetTexture(std::string textureName);
    std::optional<Mesh> GetMesh(std::string meshName);
    std::optional<ShaderProgram> GetShader(std::string shaderName);

    std::string GetResourceFolderPath();
    Texture LoadTextureFromFile(std::string filepath);
    Mesh LoadMeshFromFile(std::string filepath);
    ShaderProgram LoadShadersFromFile(std::string shaderName, std::string vertexFile, std::string fragmentFile);
};