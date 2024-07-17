#include "core/logger.h"
#define GLFW_INCLUDE_NONE
#include <common/File/File.h>
#include <core/application/application.h>
#include <core/resources/resource_system.h>
#include <core/window/input.h>
#include <core/window/window.h>
#include <core/world/components.h>
#include <core/world/scene.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace Ouroboros {

Application::Application()
    : window(Window(1280, 720)), renderer(Renderer::GetInstance()) {
  startTime = glfwGetTime();
};

void Application::Update(f64 deltaTime) {
  OUROWARN("Current running application didn't implement Update");
}

void Application::Run() {
  while (shouldRun) {
    Update(deltaTime);
  }
  return;
}

} // namespace Ouroboros