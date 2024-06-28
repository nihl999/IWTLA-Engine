#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <core/defines.h>
#include <core/resources/resource_manager.h>

const u32 InvalidTexId = -999;
struct MeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoord;
};
struct BatchMeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoord;
    u32 modelMatrixIndex;
};

struct Texture
{
    u32 id = InvalidTexId;
    i32 width;
    i32 height;
    i8 nrChannels;
    byte *data;
};

struct Material
{
    std::string shader = std::string("defaults/shaders/unlit");
    glm::vec3 tint;
    ResourceSystem::Handle diffuseMap;
    ResourceSystem::Handle specularMap;
    f32 shininess = 32.0;
};

struct Mesh
{
    MeshVertex *vertices;
    u32 verticeCount = 0;
    // todo next thing... handle handle material
    Material material;
};

Mesh LoadMeshFromFile(std::string file);
