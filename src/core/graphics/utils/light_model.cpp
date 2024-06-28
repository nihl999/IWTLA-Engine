#include "light_model.h"
#include <core/graphics/mesh.h>
#include <core/resources/resource_manager.h>

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
    scale = glm::vec3(.3, .3, .3);
    ResourceSystem::Resource *meshResource = ResourceSystem::GetResource("defaults/mesh/cube");
    if (meshResource == nullptr)
    {
        printf("cube_tex not found \n");
        exit(1);
    }
    mesh = (Mesh *)meshResource->data;
};

void LightModel::Update()
{
    HasTransform::Update();
};
