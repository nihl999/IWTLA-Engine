#include "resource_manager.h"
#include <common/stb_image.h>
#include <common/File/File.h>
#include <core/graphics/renderer/renderer.h>

ResourceManager *ResourceManager::instance = nullptr;

std::string ResourceManager::GetResourceFolderPath()
{
    // todo make dynamic...
    return "../resources/";
}

Texture ResourceManager::LoadTextureFromFile(std::string filename)
{
    Texture texture = {};

    std::string filepath = GetResourceFolderPath().append(filename);
    printf("filepath > %s\n", filepath.c_str());
    byte *data = stbi_load(filepath.c_str(), &texture.width, &texture.height, (i32 *)&texture.nrChannels, STBI_rgb_alpha);
    if (!data)
    {
        const char *failureReason = stbi_failure_reason();
        printf("error loading texture: %s\n", failureReason);
        exit(1);
    }
    // todo logging function
    texture.data = data;

    Renderer::GetInstance().CreateGPUTexture(texture);

    textures.emplace(filename, texture);
    printf("texture: %s loaded successfully, id: %d\n", filename.c_str(), texture.id);

    return texture;
};

ShaderProgram ResourceManager::LoadShadersFromFile(std::string shaderName, std::string vertexFile, std::string fragmentFile)
{
    // todo clean
    char *vertexShader = FileUtils::readFile(vertexFile.c_str());
    char *fragmentShader = FileUtils::readFile(fragmentFile.c_str());
    ShaderProgram shaderProgram = ShaderProgram();
    Renderer::GetInstance().CreateShaderProgram(vertexShader, fragmentShader, shaderProgram);
    free(vertexShader);
    free(fragmentShader);
    shaders.emplace(shaderName, shaderProgram);
    return shaderProgram;
}

ResourceManager::ResourceManager()
{
    stbi_set_flip_vertically_on_load(1);
    // note default tex maps
    LoadTextureFromFile("defaults/white_1x1.jpg");
};

ResourceManager &ResourceManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new ResourceManager();
    }
    return *instance;
};

ResourceManager::~ResourceManager()
{
    for (auto texture = textures.begin(); texture != textures.end(); texture++)
    {
        stbi_image_free(texture->second.data);
        // todo this works?
        texture->second.data = 0;
    }
};

std::optional<Texture> ResourceManager::GetTexture(std::string textureName)
{
    try
    {
        return textures.at(textureName);
    }
    catch (const std::out_of_range &oor)
    {
        printf("texture doesnt exist on resource manager\n");
        return std::nullopt;
    }
}
std::optional<Mesh> ResourceManager::GetMesh(std::string meshName)
{
    try
    {
        return meshes.at(meshName);
    }
    catch (const std::out_of_range &oor)
    {
        printf("mesh doesnt exist on resource manager\n");
        return std::nullopt;
    }
}
std::optional<ShaderProgram> ResourceManager::GetShader(std::string shaderName)
{
    try
    {
        return shaders.at(shaderName);
    }
    catch (const std::out_of_range &oor)
    {
        printf("shader doesnt exist on resource manager\n");
        return std::nullopt;
    }
}
