#pragma once
#define GLFW_INCLUDE_NONE
#include <core/defines.h>
#include <core/graphics/renderer/renderer.h>
#include <core/window/window.h>
#include <core/world/scene.h>

// todo this will change
namespace Ouroboros {
class OUROAPI Application {
protected:
  Window window = Window(1280, 720);
  Renderer renderer;
  float lastFrameTime = 0;
  float startTime = 0;

public:
  bool shouldRun = true;
  f64 deltaTime = 0;
  Scene currentScene;
  Application();

protected:
  virtual void Update(f64 deltaTime);

public:
  void Run();
};

extern Application *CreateApplication();
} // namespace Ouroboros