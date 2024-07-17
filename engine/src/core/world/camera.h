#pragma once
#include <core/defines.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OUROAPI Camera {
private:
  float moveSpeed = 5.0f;
  float nearClip = 0.1f;
  float farClip = 100.0f;
  float fov = 45.0f;
  float yaw = -90.0f;
  float pitch = 0.0f;
  float aspectRatio = ((float)16 / 9);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraRight = glm::cross(cameraUp, forward);
  glm::mat4 modelMatrix = glm::lookAt(position, position + forward, cameraUp);
  glm::mat4 projectionMatrix =
      glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

public:
  void updateCameraDir();
  void updateViewMatrix();
  void updateProjectionMatrix();

public:
  glm::vec3 position = glm::vec3(0.0f);
  glm::mat4 finalViewMatrix = projectionMatrix * modelMatrix;

public:
  Camera();
  Camera(glm::vec3 spawnPosition);
  void update();
  void updateYawPitch(float yawDelta, float pitchDelta);
  void updatePosition(glm::vec3 newPosition);
  void translate(glm::vec3 translation);
  void move(glm::vec3 directions);
  void DrawCameraPropertiesDebug();
  glm::mat4 getViewMatrix() const;
};
