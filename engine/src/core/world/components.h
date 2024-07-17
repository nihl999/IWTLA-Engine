#pragma once
#include <core/defines.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <core/graphics/model.h>
#include <core/resources/resource_system.h>

namespace ECSComponents
{
struct OUROAPI Transform {
  glm::vec3 position = glm::vec3(0);
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 rotation = glm::vec3(0);
  glm::mat4 modelMatrix = glm::mat4(1);
};
    struct OUROAPI Velocity {
      glm::vec3 velocity = glm::vec3(0);
    };

    struct OUROAPI Model {
      ResourceSystem::Handle model;
    };

    struct OUROAPI Renderable {};

    struct OUROAPI PointLight {
      f32 quadratic = 0.0075;
      f32 linear = 0.045;
      glm::vec3 color;
      f32 intensity;
    };

    struct OUROAPI DirectionalLight {
      glm::vec3 direction;
      glm::vec3 color;
      f32 intensity;
    };
};
