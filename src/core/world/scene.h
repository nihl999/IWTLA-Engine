#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <flecs/flecs.h>
#include <functional>
#include <Camera.h>

// todo probably with real ecs will set cameras as normal entities with renderer system
class Scene
{
public:
    flecs::world world;
    Camera camera;
    Scene();
    Scene(Camera cam);
    void Update();
    void DrawScenePropertiesDebug();
    void RegisterSystems();

    void RegisterMovementSystem();
    void RegisterTransformMatrixCalcSystem();
    void RegisterRenderModels();

    void Setup(std::function<void(Scene &)> lambda);
    // void RegisterLightSystem();
};
