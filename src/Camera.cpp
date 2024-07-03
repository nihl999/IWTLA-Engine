#include "Camera.h"
#include <imgui/imgui.h>

Camera::Camera(){};
Camera::Camera(glm::vec3 spawnPosition) : position(spawnPosition) {};
void Camera::updateViewMatrix()
{
    modelMatrix = glm::lookAt(
        position,
        position + forward,
        cameraUp);

    finalViewMatrix = projectionMatrix * modelMatrix;
}

void Camera::updateProjectionMatrix()
{
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::update()
{
    updateCameraDir();
    updateProjectionMatrix();
    updateViewMatrix();
}
glm::mat4 Camera::getViewMatrix() const
{
    return finalViewMatrix;
}

void Camera::DrawCameraPropertiesDebug()
{
    ImGui::Begin("Camera Properties");
    ImGui::SliderFloat("Speed", &moveSpeed, 3.0f, 5.0f);
    ImGui::DragFloat3("Camera Position", &position.x, 0.5f, 0.0f, 0.0f, "%.2f");
    ImGui::SliderFloat("Camera Yaw", &yaw, -270.0f, 90.0f, "%.2f");
    ImGui::SliderFloat("Camera Pitch", &pitch, -89.0f, 89.0f, "%.2f");
    ImGui::BeginChild("Clipping Props");
    ImGui::SliderFloat("Near Clip", &nearClip, .1f, 5.0f, "%.2f");
    ImGui::SliderFloat("Far Clip", &farClip, 20.0f, 100.0f, "%.2f");
    ImGui::SliderFloat("Fov", &fov, 10.0f, 180.0f, "%.2f");
    ImGui::EndChild();
    ImGui::End();
}

void Camera::updateYawPitch(float yawDelta, float pitchDelta)
{
    yaw += yawDelta;
    pitch += pitchDelta;
    if (pitch >= 89.9f)
    {
        pitch = 89.9f;
    }
    else if (pitch <= -89.9f)
    {
        pitch = -89.9f;
    }
    return;
}

void Camera::updatePosition(glm::vec3 newPosition)
{
    position = newPosition;
    return;
}

void Camera::translate(glm::vec3 translation)
{
    glm::vec3 newCamPos = position + translation;
    position = newCamPos;
    return;
}

void Camera::move(glm::vec3 directions)
{
    // todo try vector multiplication for this instead of add
    glm::vec3 newCamPos = position;
    directions = directions * moveSpeed;
    newCamPos += directions.x * glm::normalize(glm::cross(forward, cameraUp));
    newCamPos += directions.y * cameraUp;
    newCamPos += directions.z * forward;
    position = newCamPos;
    return;
}

void Camera::updateCameraDir()
{
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return;
}