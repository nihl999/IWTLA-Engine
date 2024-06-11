#ifndef CUBE_MODEL
#define CUBE_MODEL
#include "TempCubeMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
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

#endif