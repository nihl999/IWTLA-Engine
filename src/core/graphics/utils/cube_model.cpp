#include "cube_model.h"
#include <core/graphics/mesh.h>
#include <core/resources/resource_manager.h>

const float cube_vert_pos[108] = {
    -0.5f, -0.5f, -0.5f, //
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f};
const float cube_vert_uvs[72] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f};

const float cube_vert_normals[108] = {
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f};

void initCubeMesh()
{
    Graphics::Mesh *cubeMesh = (Graphics::Mesh *)calloc(1, sizeof(Graphics::Mesh));
    Graphics::Texture cubeTexture;
    // Graphics::MeshVertex vertices[36];
    Graphics::MeshVertex *vertices = (Graphics::MeshVertex *)calloc(1, sizeof(Graphics::MeshVertex) * 36);
    if (!vertices)
    {
        // todo logging
        printf("error callocing cube_model cubeMesh vertices\n");
        exit(1);
    }
    for (u32 i = 0; i < 36; i++)
    {
        Graphics::MeshVertex vert;
        vert.position = glm::vec3(cube_vert_pos[i * 3], cube_vert_pos[i * 3 + 1], cube_vert_pos[i * 3 + 2]);
        vert.normal = glm::vec3(cube_vert_normals[i * 3], cube_vert_normals[i * 3 + 1], cube_vert_normals[i * 3 + 2]);
        vert.texCoord = glm::vec3(cube_vert_uvs[i * 2], cube_vert_uvs[i * 2 + 1], 1);
        vertices[i] = vert;
    }

    cubeMesh->vertices = vertices;
    cubeMesh->verticeCount = 36;
    std::string file = "container.jpg";
    cubeTexture = ResourceManager::loadTextureFromFile(file);
    cubeMesh->textureName = file;

    // todo refactor - pretty manager
    ResourceManager::meshes.emplace(std::string("CubeMesh"), *cubeMesh);
}

// todo this is dumb, could do with 1 method and without last position

CubeModel::CubeModel() : CubeModel::CubeModel(glm::vec3(0))
{
    printf("Called Default CubeModel constructor\n");
    printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
};

CubeModel::CubeModel(glm::vec3 _position)
{
    printf("Called Position Setter CubeModel constructor\n");
    printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
    position = _position;
    auto meshIt = ResourceManager::meshes.find("CubeMesh");
    // todo probably can explode if mesh initialization failed or if run initCubeMesh two times... who knows.
    if (meshIt == ResourceManager::meshes.end())
    {
        initCubeMesh();
    }
        mesh = &ResourceManager::meshes.at("CubeMesh");
};

void CubeModel::Update()
{
    HasTransform::Update();
};
