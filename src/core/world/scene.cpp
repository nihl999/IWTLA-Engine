#include <core/world/scene.h>
#include <imgui/imgui.h>

Scene::Scene() : Scene::Scene(Camera(), {}) {};
Scene::Scene(Camera cam, const std::vector<Entity *> &ents) : camera(cam), entities(ents)
{
    ambientLight = {
        .color = glm::vec3(1),
        .intensity = 1.0f};
    pointLights = std::vector<World::PointLight>({{.position = glm::vec3(0, 5, 0),
                                                   .color = glm::vec3(1),
                                                   .intensity = 1.0f}});
    directionalLights = std::vector<World::DirectionalLight>();
};

void Scene::DrawScenePropertiesDebug()
{
    ImGui::Begin("Entities Properties");
    ImGui::Text("Entities Count %d", entities.size());
    if (ImGui::TreeNode("Entity List: "))
    {
        for (int n = 0; n < entities.size(); n++)
        {
            ImGui::PushID(entities[n]);
            if (ImGui::TreeNode(entities[n], "Entity %d", n))
            {
                ImGui::DragFloat3("Pos", &entities[n]->position[0], 0.5f, 0.0f, 0.0f, "%.2f");
                ImGui::DragFloat3("Sca", &entities[n]->scale[0], 0.5f, 0.0f, 0.0f, "%.2f");
                ImGui::DragFloat3("Rot", &entities[n]->rotation[0], 0.5f, 0.0f, 0.0f, "%.2f");
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::End();
    ImGui::Begin("WIP: Light Properties");
    ImGui::Text("Point Light Count %d", pointLights.size());
    if (ImGui::TreeNode("Light List: "))
    {
        for (int n = 0; n < pointLights.size(); n++)
        {
            ImGui::PushID(&pointLights[n]);
            if (ImGui::TreeNode(&pointLights[n], "Light %d", n))
            {
                ImGui::DragFloat3("Pos", &pointLights[n].position[0], 0.5f, 0.0f, 0.0f, "%.2f");
                ImGui::ColorPicker3("Color", &pointLights[n].color[0]);
                ImGui::SliderFloat("Intensity", &pointLights[n].intensity, 0.0f, 1.0f, "%.02f");
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

void Scene::Update()
{
    for (Entity *ent : entities)
    {
        ent->Update();
    }
}
