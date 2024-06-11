#include "TempModel.h"

CubeModel::CubeModel()
{
    printf("Called Default CubeModel constructor\n");
    printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
};

CubeModel::CubeModel(glm::vec3 _position) : position(_position)
{
    printf("Called Position Setter CubeModel constructor\n");
    printf("x=%f y=%f z=%f\n", position.x, position.y, position.z);
}

const CubeMesh CubeModel::mesh = CubeMesh();

// todo this is dumb, could do with 1 method and without last position
void CubeModel::RecalculateTranslationMatrix()
{
    if (lastPosition == position)
        return;
    translationMatrix = glm::translate(glm::mat4(1), position);
    lastPosition = position;
}

glm::mat4 CubeModel::GetModelMatrix()
{
    RecalculateTranslationMatrix();
    glm::mat4 model = translationMatrix * scaleMatrix * rotationMatrix;
    return model;
}