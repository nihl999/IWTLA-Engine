// #include "cube_model.h"
// #include <core/graphics/mesh.h>
// #include <core/resources/resource_manager.h>

// CubeModel::CubeModel() : CubeModel::CubeModel(glm::vec3(0))
// {
//     printf("Called Default CubeModel constructor\n");
//     printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
// };

// CubeModel::CubeModel(glm::vec3 _position)
// {
//     printf("Called Position Setter CubeModel constructor\n");
//     printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
//     position = _position;
//     ResourceSystem::Resource *meshResource = ResourceSystem::GetResource("defaults/mesh/cube_tex");
//     if (meshResource == nullptr)
//     {
//         printf("cube_tex not found \n");
//         exit(1);
//     }
//     mesh = (Mesh *)meshResource->data;
// };

// void CubeModel::Update()
// {
//     HasTransform::Update();
// };
