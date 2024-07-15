#pragma once
#define GLFW_INCLUDE_NONE
#include <core/defines.h>
#include <core/graphics/renderer/renderer.h>
#include <core/window/window.h>
#include <core/world/scene.h>

namespace Ouroboros {
class OUROAPI Application {
private:
  Window window;
  Renderer renderer;
  float lastFrameTime = 0;
  float startTime = 0;

public:
  bool shouldRun = true;
  f64 deltaTime = 0;
  Scene currentScene;
  Application();

private:
  void DrawApplicationPropertiesDebug();
  void Update();

public:
  void Run();
};

Application *CreateApplication();
} // namespace Ouroboros