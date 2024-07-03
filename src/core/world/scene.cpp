#include <core/world/scene.h>
#include <core/world/components.h>
#include <core/graphics/renderer/renderer.h>
#include <imgui/imgui.h>

// void RegisterMovementSystem(const Scene &scene);
// void RegisterTransformMatrixCalcSystem(const Scene &scene);
// void RegisterRenderModels(Scene &scene);

Scene::Scene() : Scene::Scene(Camera()) {};
Scene::Scene(Camera cam) : camera(cam) {};

void Scene::Setup(std::function<void(Scene &)> lambda)
{
    lambda(*this);
};

void Scene::RegisterSystems()
{
    RegisterMovementSystem();
    RegisterTransformMatrixCalcSystem();
    RegisterRenderModels();
};
using namespace ECSComponents;

void Scene::DrawScenePropertiesDebug()
{
    ImGui::Begin("Entities Properties");
    ImGui::Text("Entities Count %d", world.count<Renderable>());
    if (ImGui::TreeNode("Entity List: "))
    {
        u32 ent = 0;
        u32 lightes = 0;
        world.each([&](flecs::entity e, Transform &t)
                   {
                       if (e.has<PointLight>())
                           return;
                       ImGui::PushID(e.id());
                       if (ImGui::TreeNode(&ent, "Entity %d", ent))
                       {
                           ImGui::DragFloat3("Pos", &t.position[0], 0.5f, 0.0f, 0.0f, "%.2f");
                           ImGui::DragFloat3("Sca", &t.scale[0], 0.5f, 0.0f, 0.0f, "%.2f");
                           ImGui::DragFloat3("Rot", &t.rotation[0], 0.5f, 0.0f, 0.0f, "%.2f");
                           ImGui::TreePop();
                       }
                       ent++;
                       ImGui::PopID();

                       // unbreak
                   });
        world.each([&](flecs::entity e, Transform &t, PointLight &light)
                   {
                       ImGui::PushID(e.id());
                       if (ImGui::TreeNode(&lightes, "Light %d", lightes))
                       {
                           ImGui::DragFloat3("Pos", &t.position[0], 0.5f, 0.0f, 0.0f, "%.2f");
                           ImGui::DragFloat3("Sca", &t.scale[0], 0.5f, 0.0f, 0.0f, "%.2f");
                           ImGui::DragFloat3("Rot", &t.rotation[0], 0.5f, 0.0f, 0.0f, "%.2f");
                           ImGui::ColorPicker3("Color", &light.color[0]);
                           ImGui::SliderFloat("Intensity", &light.intensity, 0.0f, 1.0f, "%.02f");
                           ImGui::TreePop();
                       }
                       lightes++;
                       ImGui::PopID();
                       // unbreak
                   });

        ImGui::TreePop();
    }

    ImGui::End();
}

void Scene::Update()
{
    world.progress();

    camera.update();
}

void Scene::RegisterMovementSystem()
{
    flecs::system move_sys = world.system<Transform, Velocity>()
                                 .each([](Transform &t, Velocity &v)
                                       {
                                           t.position.x += v.velocity.x;
                                           t.position.y += v.velocity.y;
                                           t.position.z += v.velocity.z;
                                           // unbreak
                                       });
    move_sys.add(flecs::OnUpdate);
}

// void Scene::RegisterLightSystem()
// {
//     flecs::system move_sys = world.system<Transform, Light>()
//                                  .each([](Transform &t, Light &v)
//                                        {
//                                        });
//     move_sys.add(flecs::OnUpdate);
// }

void Scene::RegisterTransformMatrixCalcSystem()
{
    glm::mat4 identity = glm::mat4(1);
    flecs::system move_sys = world.system<Transform>()
                                 .each([identity](Transform &t)
                                       {
                                           glm::mat4 translationMatrix = glm::translate(identity, t.position);
                                           glm::mat4 scaleMatrix = glm::scale(identity, t.scale);
                                           f32 xAngle = t.rotation.x;
                                           f32 yAngle = t.rotation.y;
                                           f32 zAngle = t.rotation.z;
                                           glm::mat4 rotationMatrix = glm::rotate(identity, glm::radians(xAngle), glm::vec3(1, 0, 0));
                                           rotationMatrix *= glm::rotate(identity, glm::radians(yAngle), glm::vec3(0, 1, 0));
                                           rotationMatrix *= glm::rotate(identity, glm::radians(zAngle), glm::vec3(0, 0, 1));
                                           t.modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
                                           //    printf("x: %f y: %f z: %f\n", t.position.x, t.position.y, t.position.z);
                                       });
    move_sys.add(flecs::OnUpdate);
}

void Scene::RegisterRenderModels()
{
    auto render_sys = world.system<Transform, ECSComponents::Model, Renderable>()
                          .each([this](Transform &t, ECSComponents::Model &m, Renderable r)
                                {
                                    Renderer renderer = Renderer::GetInstance();
                                    ResourceSystem::Resource *modelResource = ResourceSystem::GetResource(m.model);
                                    if (modelResource == nullptr)
                                    {
                                        printf("model is nullptr, and shouldnt...\n");
                                        return;
                                    }
                                    Graphics::Model *model = (Graphics::Model *)modelResource->data;
                                    printf("x: %f y: %f z: %f\n", t.position.x, t.position.y, t.position.z);
                                    auto light = world.filter<Transform, PointLight>().first();
                                    const Transform *lightPos = light.get<Transform>();
                                    const PointLight *lightC = light.get<PointLight>();
                                    for (Graphics::Mesh mesh : model->meshes)
                                    {
                                        ResourceSystem::Resource *materialResource = ResourceSystem::GetResource(mesh.material);
                                        if (materialResource == nullptr)
                                        {
                                            printf("shader not loaded \n");
                                            exit(1);
                                        }
                                        Material *material = (Material *)materialResource->data;
                                        ResourceSystem::Resource *shaderResource = ResourceSystem::GetResource(material->shader);
                                        if (shaderResource == nullptr)
                                        {
                                            printf("shader not loaded \n");
                                            exit(1);
                                        }
                                        ShaderProgram *shader = (ShaderProgram *)shaderResource->data;
                                        renderer.BindShaderProgram(*shader);
                                        renderer.UniformFMat4("view_projection_matrix", camera.getViewMatrix());
                                        renderer.UniformF1("ambient_light_force", lightC->intensity);
                                        renderer.UniformFVec3("point_light_position",
                                                              lightPos->position);
                                        renderer.UniformFVec3("point_light_color",
                                                              lightC->color);
                                        renderer.UniformFVec3("cam_pos",
                                                              camera.position);
                                        renderer.UniformFMat4("model_matrix",
                                                              t.modelMatrix);

                                        renderer.RenderMesh(mesh);
                                    }
                                    // unbreak
                                });
    render_sys.add(flecs::OnStore);
}