#include "light_model.h"
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

void initLightMesh()
{
    Mesh *lightMesh = (Mesh *)calloc(1, sizeof(Mesh));
    Texture cubeTexture;
    // MeshVertex vertices[36];
    MeshVertex *vertices = (MeshVertex *)calloc(1, sizeof(MeshVertex) * 36);
    if (!vertices)
    {
        // todo logging
        printf("error callocing cube_model cubeMesh vertices\n");
        exit(1);
    }
    for (u32 i = 0; i < 36; i++)
    {
        MeshVertex vert;
        vert.position = glm::vec3(cube_vert_pos[i * 3], cube_vert_pos[i * 3 + 1], cube_vert_pos[i * 3 + 2]);
        vert.normal = glm::vec3(cube_vert_normals[i * 3], cube_vert_normals[i * 3 + 1], cube_vert_normals[i * 3 + 2]);
        vert.texCoord = glm::vec3(cube_vert_uvs[i * 2], cube_vert_uvs[i * 2 + 1], 1);
        vertices[i] = vert;
    }

    lightMesh->vertices = vertices;
    lightMesh->verticeCount = 36;
    Material mat = {.tint = glm::vec3(1),
                    .diffuseMap = std::string("white_1x1.jpg"),
                    .specularMap = std::string("white_1x1.jpg"),
                    .shader = std::string("unlit")};
    lightMesh->material = mat;

    ResourceManager::GetInstance().meshes.emplace(std::string("LightMesh"), *lightMesh);
}

LightModel::LightModel() : LightModel::LightModel(glm::vec3(0))
{
    printf("Called Default CubeModel constructor\n");
    printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
};

LightModel::LightModel(glm::vec3 _position)
{
    printf("Called Position Setter CubeModel constructor\n");
    printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
    position = _position;
    auto meshIt = ResourceManager::GetInstance().meshes.find("LightMesh");
    // todo probably can explode if mesh initialization failed or if run initCubeMesh two times... who knows.
    if (meshIt == ResourceManager::GetInstance().meshes.end())
    {
        initLightMesh();
    }
    mesh = &ResourceManager::GetInstance().meshes.at("LightMesh");
};

void LightModel::Update()
{
    HasTransform::Update();
};
