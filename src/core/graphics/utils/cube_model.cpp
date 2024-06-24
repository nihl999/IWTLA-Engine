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
    Mesh *cubeMesh = (Mesh *)calloc(1, sizeof(Mesh));
    Texture diffTexture;
    Texture specTexture;
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

    cubeMesh->vertices = vertices;
    cubeMesh->verticeCount = 36;
    std::string diffTexFile = "container.png";
    std::string specTexFile = "container_specular.png";
    diffTexture = ResourceManager::GetInstance().LoadTextureFromFile(diffTexFile);
    specTexture = ResourceManager::GetInstance().LoadTextureFromFile(specTexFile);
    Material mat = {.tint = glm::vec3(1),
                    .diffuseMap = diffTexFile,
                    .specularMap = specTexFile};
    cubeMesh->material = mat;

    ResourceManager::GetInstance().meshes.emplace(std::string("CubeMesh"), *cubeMesh);
}

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
    auto meshIt = ResourceManager::GetInstance().meshes.find("CubeMesh");
    // todo probably can explode if mesh initialization failed or if run initCubeMesh two times... who knows.
    if (meshIt == ResourceManager::GetInstance().meshes.end())
    {
        initCubeMesh();
    }
    mesh = &ResourceManager::GetInstance().meshes.at("CubeMesh");
};

void CubeModel::Update()
{
    HasTransform::Update();
};
