#ifndef CUBE_MESH_IMPL
#define CUBE_MESH_IMPL
#include "core/defines.h"
class CubeMesh
{
public:
    // todo ... nothing to say
    static const float vertices[108];
    static const float uvs[72];
    static const float normals[108];
    static byte *tex;
    static int width, height, nrChannels;
    CubeMesh();
};
#endif