#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "../defines.h"

struct MeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoord;
};

struct Texture
{
    u32 id;
    u32 width;
    u32 height;
    u8 nrChannels;
    std::shared_ptr<byte> data;
};

struct Mesh
{
public:
    MeshVertex *vertices;
    // todo implement texture and material resources ---
    u32 texId;
};