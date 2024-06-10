#include "TempCubeMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#ifndef MODEL_IMPL
#define MODEL_IMPL
class CubeModel
{
public:
    const static CubeMesh mesh;
    // todo should be on some entity
    glm::vec3 position = glm::vec3(0);
    glm::vec3 lastPosition = glm::vec3(0);
    glm::mat4 translationMatrix = glm::mat4(1);
    glm::mat4 scaleMatrix = glm::mat4(1);
    glm::mat4 rotationMatrix = glm::mat4(1);

    glm::mat4 GetModelMatrix();
    CubeModel();
    CubeModel(glm::vec3 position);

private:
    void RecalculateTranslationMatrix();
};

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

#endif