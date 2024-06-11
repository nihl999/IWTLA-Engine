#include "Scene.h"

Scene::Scene() : camera(Camera()), entities({CubeModel(), CubeModel(glm::vec3(5, 0, -5))}){};
Scene::Scene(Camera &cam, auto &ents) : camera(cam), entities(ents){};
