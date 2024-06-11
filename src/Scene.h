#ifndef SCENE_CLASS
#define SCENE_CLASS
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "TempModel.h"

// todo probably with real ecs will set cameras as normal entities with renderer system
class Scene
{
public:
    std::vector<CubeModel> entities;
    Camera camera;
    Scene();
    Scene(Camera &cam, auto &ents);
};

#endif