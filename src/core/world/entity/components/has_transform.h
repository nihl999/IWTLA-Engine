#pragma once
#include <core/defines.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// todo dirty flag optimization - wasScaled, wasTranslated, wasRotated
class HasTransform
{
public:
    glm::vec3 lastPosition = glm::vec3(0);
    glm::vec3 lastScale = glm::vec3(0);
    glm::vec3 lastRotation = glm::vec3(0);
    glm::vec3 position = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 rotation = glm::vec3(0);
    glm::mat4 modelMatrix = glm::mat4(1);

private:
    glm::mat4 translationMatrix = glm::mat4(1);
    glm::mat4 scaleMatrix = glm::mat4(1);
    glm::mat4 rotationMatrix = glm::mat4(1);
    void CalculateTranslationMatrix();
    void CalculateRotationMatrix();
    void CalculateScaleMatrix();

protected:
    void Update();
};
