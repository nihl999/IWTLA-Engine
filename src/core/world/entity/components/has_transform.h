#pragma once
#include <glm/glm.hpp>

class HasTransform
{
public:
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);
    glm::mat4 modelMatrix = glm::mat4(1);

private:
    glm::mat4 translationMatrix = glm::mat4(1);
    glm::mat4 scaleMatrix = glm::mat4(1);
    glm::mat4 rotationMatrix = glm::mat4(1);
};

// void CubeModel::RecalculateTranslationMatrix()
// {
//     if (lastPosition == position)
//         return;
//     translationMatrix = glm::translate(glm::mat4(1), position);
//     lastPosition = position;
// }

// glm::mat4 CubeModel::GetModelMatrix()
// {
//     RecalculateTranslationMatrix();
//     glm::mat4 model = translationMatrix * scaleMatrix * rotationMatrix;
//     return model;
// }