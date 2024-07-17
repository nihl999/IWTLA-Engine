#pragma once

#include "core/defines.h"
#include <core/world/camera.h>
#include <flecs/flecs.h>
#include <functional>
#include <glm/glm.hpp>

class OUROAPI Scene {
public:
  flecs::world world;
  Camera camera;
  Scene();
  void Update();
  void DrawScenePropertiesDebug();

  void Setup(std::function<void(Scene &)> lambda);
  void RegisterSystems();
  void RegisterMovementSystem();
  void RegisterTransformMatrixCalcSystem();
  void RegisterRenderModels();
};
