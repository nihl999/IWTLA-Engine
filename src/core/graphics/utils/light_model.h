#pragma once
#include "core/defines.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <core/world/entity/entity.h>
#include <core/world/entity/components/has_transform.h>
#include <core/world/entity/components/has_mesh.h>

class LightModel : public Entity, public HasMesh
{
public:
    LightModel();
    LightModel(glm::vec3 _position);

private:
    void Update();
};
