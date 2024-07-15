#define GLFW_INCLUDE_NONE
#include <common/File/File.h>
#include <core/application/application.h>
#include <core/graphics/utils/cube_model.h>
#include <core/graphics/utils/light_model.h>
#include <core/resources/resource_system.h>
#include <core/window/input.h>
#include <core/window/window.h>
#include <core/world/components.h>
#include <core/world/scene.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <random>

namespace Ouroboros {
u32 frames = 0;
f64 lastFPSCalc;
void Application::DrawApplicationPropertiesDebug() {
  ImGui::Begin("Application Properties");
  ImGui::Text("Frame time: %f ms", deltaTime * 1000);
  ImGui::Checkbox("Capture Mouse", &Input::capturingMouse);
  // todo Should be on some inputs tab maybe? or application handles... anyway
  ImGui::SliderFloat("Mouse Sensitivity", &Input::sensitivity, 0.01f, 3.0f,
                     "%.2f");
  ImGui::Text("Mouse X: %.2f", Input::mousePositionX);
  ImGui::Text("Mouse Y: %.2f", Input::mousePositionY);
  ImGui::Text("Mouse X Offset: %.3f", Input::mouseDeltaX);
  ImGui::Text("Mouse Y Offset: %.3f", Input::mouseDeltaY);
  ImGui::End();
  currentScene.DrawScenePropertiesDebug();
}

Application::Application()
    : window(Window(1280, 720)), renderer(Renderer::GetInstance()) {
  ResourceSystem::Init();
  currentScene = Scene();
  // currentScene = Scene(Camera(glm::vec3(0, 0, 10)));
  currentScene.Setup([](Scene &scene) {
    // Random seed
    std::random_device rd;

    // Initialize Mersenne Twister pseudo-random number generator
    std::mt19937 gen(rd());

    std::uniform_real_distribution<f32> x(-10, 30);
    std::uniform_real_distribution<f32> y(0, 10);
    std::uniform_real_distribution<f32> z(-50, -10);

    for (u32 i = 0; i <= 10; i++) {
      flecs::entity cube = scene.world.entity();
      cube.set<ECSComponents::Transform>(
          {.position = glm::vec3(x(gen), y(gen), z(gen))});
      cube.add<ECSComponents::Velocity>();
      cube.add<ECSComponents::Renderable>();
      cube.set<ECSComponents::Model>(
          {.model = ResourceSystem::PrepareResource(
               {.path = "hardcoded/cube",
                .name = "defaults/model/cube_tex",
                .type = ResourceSystem::RESOURCE_MODEL})});
    };

    {
      flecs::entity light = scene.world.entity("directionallight");
      light.set<ECSComponents::DirectionalLight>(
          {.direction = glm::vec3(0, -0.5, -1),
           .color = glm::vec3(1),
           .intensity = 1.0f});
      light.add<ECSComponents::Transform>();
    }
    {
      flecs::entity light = scene.world.entity("pointlight");
      light.set<ECSComponents::Transform>({.position = glm::vec3(0, 0, 0)});
      light.add<ECSComponents::Velocity>();
      light.add<ECSComponents::Renderable>();
      light.set<ECSComponents::PointLight>(
          {.color = glm::vec3(1), .intensity = 1.0f});

      light.set<ECSComponents::Model>(
          {.model = ResourceSystem::PrepareResource(
               {.path = "hardcoded/cube",
                .name = "defaults/model/cube",
                .type = ResourceSystem::RESOURCE_MODEL})});
    }
    // lambda end
  });
  //   currentScene.RegisterSystems();
  startTime = glfwGetTime();
  lastFPSCalc = glfwGetTime();
}

void Application::Update() {
  if (window.ShouldClose()) {
    shouldRun = false;
  }
  window.PoolEvents();
  deltaTime = glfwGetTime() - lastFrameTime;
  lastFrameTime = glfwGetTime();
  frames++;

  window.NewFrame();
  renderer.NewFrame();

  // currentScene.camera.move(glm::vec3(Input::axis["Horizontal"].currentAxisValue
  // * deltaTime, Input::axis["Fly"].currentAxisValue * deltaTime,
  // Input::axis["Vertical"].currentAxisValue * deltaTime));
  //   currentScene.camera.updateYawPitch(Input::mouseDeltaX,
  //   Input::mouseDeltaY);
  currentScene.Update();

  DrawApplicationPropertiesDebug();
  //   currentScene.camera.DrawCameraPropertiesDebug();

  renderer.EndFrame();

  window.SwapBuffers();
  // todo dont realy want to poll mouse position, but it will be what it will be

  if (lastFrameTime - lastFPSCalc >= 1) {
    // printf("%f ms/frame\n", 1000.0 / double(frames));
    lastFPSCalc = lastFrameTime;
    frames = 0;
  }
  Input::mouseDeltaX = 0;
  Input::mouseDeltaY = 0;
}

void Application::Run() {
  while (shouldRun) {
    Update();
  }
  return;
}
} // namespace Ouroboros