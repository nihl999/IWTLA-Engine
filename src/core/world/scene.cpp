#include <core/world/scene.h>

Scene::Scene() : Scene::Scene(Camera(), {}) {};
Scene::Scene(Camera cam, const std::vector<Entity> &ents) : camera(cam), entities(ents)
{
    ambientLight = {
        .color = glm::vec3(1),
        .intensity = 1.0f};
    pointLights = std::vector<World::PointLight>();
    directionalLights = std::vector<World::DirectionalLight>();
};
