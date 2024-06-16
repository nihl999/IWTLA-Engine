#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <core/world/light.h>
#include <Camera.h>
#include <core/graphics/utils/cube_model.h>

// todo probably with real ecs will set cameras as normal entities with renderer system
class Scene
{
public:
    std::vector<Entity> entities;
    std::vector<World::PointLight> pointLights;
    std::vector<World::DirectionalLight> directionalLights;
    World::AmbientLight ambientLight;
    Camera camera;
    Scene();
    Scene(Camera cam, const std::vector<Entity> &ents);
};
