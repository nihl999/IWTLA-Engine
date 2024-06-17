#include <core/world/entity/components/has_transform.h>

void HasTransform::CalculateTranslationMatrix()
{
    if (lastPosition == position)
        return;
    translationMatrix = glm::translate(glm::mat4(1), position);
    lastPosition = position;
}

void HasTransform::CalculateRotationMatrix()
{
    if (lastRotation == rotation)
        return;
    f32 xAngle = rotation.x;
    f32 yAngle = rotation.y;
    f32 zAngle = rotation.z;
    glm::mat4 identity = glm::mat4(1);
    rotationMatrix = glm::rotate(identity, glm::radians(xAngle), glm::vec3(1, 0, 0));
    rotationMatrix *= glm::rotate(identity, glm::radians(yAngle), glm::vec3(0, 1, 0));
    rotationMatrix *= glm::rotate(identity, glm::radians(zAngle), glm::vec3(0, 0, 1));
    lastRotation = rotation;
}

void HasTransform::CalculateScaleMatrix()
{
    if (lastScale == scale)
        return;
    scaleMatrix = glm::scale(glm::mat4(1), scale);
    lastScale = scale;
}

void HasTransform::Update()
{
    CalculateRotationMatrix();
    CalculateTranslationMatrix();
    CalculateScaleMatrix();
    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
}