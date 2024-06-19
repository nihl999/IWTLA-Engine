#include "resource_manager.h"
#include <common/stb_image.h>
#include <common/File/File.h>
#include <core/renderer/renderer.h>

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
    byte *data = stbi_load(filepath.c_str(), &texture.width, &texture.height, (i32 *)&texture.nrChannels, 0);
    if (!data)
    {
        const char *failureReason = stbi_failure_reason();
        printf("error loading texture: %s\n", failureReason);
        exit(1);
    }
    // todo logging function
    texture.data = std::make_shared<byte *>(data);
    textures.emplace(filename, texture);
    printf("texture: %s loaded successfully, id: %d\n", filename.c_str(), texture.id);

    Renderer::GetInstance().CreateGPUTexture(texture);

    return texture;
};

ShaderProgram ResourceManager::LoadShadersFromFile(std::string vertexFile, std::string fragmentFile)
{
    // todo clean
    char *vertexShader = FileUtils::readFile(vertexFile.c_str());
    char *fragmentShader = FileUtils::readFile(fragmentFile.c_str());
    i32 programId = Renderer::GetInstance().CreateShaderProgram(vertexShader, fragmentShader);
    free(vertexShader);
    free(fragmentShader);
    ShaderProgram shaderProgram = ShaderProgram();
    shaderProgram.id = programId;
    return shaderProgram;
}

ResourceManager::ResourceManager()
{
    stbi_set_flip_vertically_on_load(1);
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
        stbi_image_free(*texture->second.data);
        // todo this works?
        texture->second.data.reset();
    }
};
