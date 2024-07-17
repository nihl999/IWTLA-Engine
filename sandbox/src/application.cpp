#include "core/application/application.h"
#include "core/resources/resource_system.h"
#include <imgui/imgui.h>
#include <ouroboros.h>
#include <random>

class Sandbox : public Ouroboros::Application {
public:
  Sandbox();
  ~Sandbox() = default;
  void Update(f64 deltaTime) override;

private:
  void DrawApplicationPropertiesDebug();
  f32 frames;
  f32 lastFPSCalc;
};

void Sandbox::DrawApplicationPropertiesDebug() {
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

Sandbox::Sandbox() : Ouroboros::Application() {
  ResourceSystem::Init();
  currentScene = Scene();
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
  currentScene.RegisterSystems();
}

void Sandbox::Update(f64 deltaTime) {
  if (window.ShouldClose()) {
    shouldRun = false;
  }
  window.PoolEvents();
  deltaTime = glfwGetTime() - lastFrameTime;
  lastFrameTime = glfwGetTime();
  frames++;

  window.NewFrame();
  renderer.NewFrame();

  currentScene.camera.move(
      glm::vec3(Input::axis["Horizontal"].currentAxisValue * deltaTime,
                Input::axis["Fly"].currentAxisValue * deltaTime,
                Input::axis["Vertical"].currentAxisValue * deltaTime));
  currentScene.camera.updateYawPitch(Input::mouseDeltaX, Input::mouseDeltaY);
  currentScene.Update();

  DrawApplicationPropertiesDebug();
  currentScene.camera.DrawCameraPropertiesDebug();

  renderer.EndFrame();

  window.SwapBuffers();

  if (lastFrameTime - lastFPSCalc >= 1) {
    lastFPSCalc = lastFrameTime;
    frames = 0;
  }
  Input::mouseDeltaX = 0;
  Input::mouseDeltaY = 0;
}

Ouroboros::Application *Ouroboros::CreateApplication() { return new Sandbox(); }