#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Snow/Scene/Components.h"

#include "Snow/Script/ScriptEngine.h"

#include <glm/gtc/type_ptr.hpp>

#include "Snow/Utils/FileDialogs.h"

#include "UI/ImGuiUI.h"

namespace Snow {
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
        m_CheckerboardTexture = Render::API::Texture2D::Create("assets/textures/Checkerboard.png");

        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) {
        m_SceneContext = scene;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender() {
        

        if (m_SceneContext) {
            ImGui::Begin("Scene Hierarchy");
            uint32_t entityCount = 0;

            m_SceneContext->m_Registry.each([&](auto entity) {
                Entity e{ entity, m_SceneContext.Raw() };
                if(e.HasComponent<IDComponent>())
                    DrawEntityNode(e);
            });

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                m_SelectionContext = {};

            if (ImGui::BeginPopupContextWindow(0, 1, false)) {
                if (ImGui::MenuItem("Create empty entity"))
                    m_SceneContext->CreateEntity("Empty Entity");

                ImGui::EndPopup();
            }

            ImGui::End();

            ImGui::Begin("Properties");
            if (m_SelectionContext)
                DrawComponents(m_SelectionContext);

            ImGui::End();

            ImGui::Begin("Environment");
            DrawEnvironment();
            ImGui::End();
        }
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
            m_SelectionContext = entity;

        if (opened) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
    }

    

    void SceneHierarchyPanel::DrawIconedImage(const std::string& label, Ref<Render::API::Texture2D>& texture) {
        ImGui::PushID(label.c_str());

        void* textureID = texture.Raw() == nullptr ? (void*)m_CheckerboardTexture->GetRendererID() : (void*)texture->GetRendererID();

        ImGui::Image(textureID, ImVec2(64, 64), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
        if (ImGui::IsItemClicked()) {
            std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("");
            if (filepath.has_value()) {
                texture = Render::API::Texture2D::Create(filepath.value());
            }
        }

        ImGui::SameLine();
        ImGui::Text(label.c_str());

        ImGui::PopID();
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>()) {
            ImGui::PushID((void*)typeid(T).hash_code());

            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();

            bool open = ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(T).hash_code()), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();

            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup("ComponentSettings");
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) {
                uiFunction(component);
                ImGui::NextColumn();
                ImGui::Columns(1);
                ImGui::TreePop();
            }
            

            if (removeComponent)
                entity.RemoveComponent<T>();

            ImGui::PopID();
        }
    }


    void SceneHierarchyPanel::DrawComponents(Entity entity) {
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        bool removeEntity = false;
        if (ImGui::Button("Delete Entity"))
            removeEntity = true;

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Camera")) {
                m_SelectionContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite Renderer")) {
                m_SelectionContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Mesh")) {
                m_SelectionContext.AddComponent<MeshComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Script")) {
                m_SelectionContext.AddComponent<ScriptComponent>("ExampleScriptApp.Controller");
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("BRDF Material")) {
                Render::PipelineSpecification pipelineSpec;
                pipelineSpec.Layout = {
                    { Render::AttribType::Float3, "a_Position" },
                    { Render::AttribType::Float3, "a_Normal" },
                    { Render::AttribType::Float3, "a_Tangent" },
                    { Render::AttribType::Float3, "a_Bitangent" },
                    { Render::AttribType::Float2, "a_TexCoord" }
                };
                pipelineSpec.Shaders = { Render::Shader::Create(Render::ShaderType::Vertex, "assets/shaders/glsl/PBRVert.glsl"), Render::Shader::Create(Render::ShaderType::Pixel, "assets/shaders/glsl/PBRFrag.glsl") };
                pipelineSpec.Type = Render::PrimitiveType::Triangle;
                Ref<Render::Pipeline> pipeline = Render::Pipeline::Create(pipelineSpec);
                Ref<Render::MaterialInstance> m_MaterialInstance = Ref<Render::MaterialInstance>::Create(Ref<Render::Material>::Create(pipeline));
                m_SelectionContext.AddComponent<BRDFMaterialComponent>(m_MaterialInstance);
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("RigidBody")) {
                auto transform = m_SelectionContext.GetComponent<TransformComponent>();
                auto rb = RigidBody2D(m_SceneContext->GetPhysicsWorld(), transform.GetTransform());
                m_SelectionContext.AddComponent<RigidBody2DComponent>(rb);
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();

        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {
            UI::DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            UI::DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            UI::DrawVec3Control("Scale", component.Scale, 1.0f);
            component.UpdateTransform();
        });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary Camera", &component.Primary);

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
            
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                for (int i = 0; i < 2; i++) {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
                float fov = glm::degrees(camera.GetPerspectiveVerticalFOV());
                if (ImGui::DragFloat("FOV", &fov))
                    camera.SetPerspectiveVerticalFOV(glm::radians(fov));

                float nearClip = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near Clip", &nearClip))
                    camera.SetPerspectiveNearClip(nearClip);

                float farClip = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far Clip", &farClip))
                    camera.SetPerspectiveFarClip(farClip);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
                float orthoSize = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &orthoSize))
                    camera.SetOrthographicSize(orthoSize);

                float nearClip = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near Clip", &nearClip))
                    camera.SetOrthographicNearClip(nearClip);

                float farClip = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far Clip", &farClip))
                    camera.SetOrthographicFarClip(farClip);
            }
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [this](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

            Ref<Render::API::Texture2D> sprite = component.Texture;
            void* textureID = sprite.Raw() == nullptr ? (void*)m_CheckerboardTexture->GetRendererID() : (void*)sprite->GetRendererID();

            ImGui::Image(textureID, ImVec2(64, 64), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
            if (ImGui::IsItemClicked()) {
                std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("");
                if (filepath.has_value()) {
                    component.Texture = Render::API::Texture2D::Create(filepath.value());
                }
            }
            
        });

        DrawComponent<MeshComponent>("Mesh", entity, [](auto& component) {
            Ref<Render::Mesh> m = component.Mesh;
            auto p = m.Raw() == nullptr ? "assets/models/Cube.obj" : m->GetPath();
            ImGui::Text(p.c_str());
            if (ImGui::IsItemClicked()) {
                std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("");
                if (filepath.has_value()) {
                    component.Mesh = Ref<Render::Mesh>::Create(filepath.value());
                }
            }
        });

        DrawComponent<BRDFMaterialComponent>("Material", entity, [this](auto& component) {
            Ref<Render::MaterialInstance> mat = component.MaterialInstance;

            if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
                bool albedoMap = component.AlbedoInput.UseTexture;
                DrawIconedImage("Albedo Tex", component.AlbedoInput.AlbedoTexture);
                ImGui::SameLine();
                ImGui::BeginGroup();
                if (ImGui::Checkbox("Use##AlbedoTexture", &albedoMap))
                    component.AlbedoInput.UseTexture = albedoMap;
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::ColorEdit3("Albedo Color", glm::value_ptr(component.AlbedoInput.AlbedoColor));
            }

            if (ImGui::CollapsingHeader("Normal", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
                bool normalMap = component.NormalInput.UseTexture;
                DrawIconedImage("Normal Tex", component.NormalInput.NormalTexture);
                ImGui::SameLine();
                if (ImGui::Checkbox("Use Normal Texture", &normalMap))
                    component.NormalInput.UseTexture = normalMap;
            }

            if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
                float& value = component.MetalnessInput.Value;
                bool metalMap = component.MetalnessInput.UseTexture;
                DrawIconedImage("Metalness", component.MetalnessInput.MetalnessTexture);
                ImGui::SameLine();
                if(ImGui::Checkbox("Use Texture", &metalMap))
                    component.MetalnessInput.UseTexture = metalMap;
                ImGui::SliderFloat("Value##MetalnessValue", &value, 0.0f, 1.0f);
            }

            if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
                float& value = component.RoughnessInput.Value;
                bool roughMap = component.RoughnessInput.UseTexture;
                DrawIconedImage("Roughness", component.RoughnessInput.RoughnessTexture);
                ImGui::SameLine();
                if (ImGui::Checkbox("Use Texture", &roughMap))
                    component.RoughnessInput.UseTexture = roughMap;
                ImGui::SliderFloat("Value##RoughnessValue", &value, 0.0f, 1.0f);
            }
        });

        DrawComponent<ScriptComponent>("Script", entity, [=](ScriptComponent& component) mutable {
            UI::BeginGrid(2);
            std::string oldName = component.ModuleName;

            if (UI::DrawStringControl("Module Name", component.ModuleName, !Script::ScriptEngine::ModuleExists(component.ModuleName))) {
                if (Script::ScriptEngine::ModuleExists(oldName))
                    Script::ScriptEngine::ShutdownScriptEntity(entity, oldName);

                if (Script::ScriptEngine::ModuleExists(component.ModuleName))
                    Script::ScriptEngine::InitScriptEntity(entity);
            }

            if (Script::ScriptEngine::ModuleExists(component.ModuleName)) {
                Script::EntityInstanceData& entityInstanceData = Script::ScriptEngine::GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID());
                auto& moduleFieldMap = entityInstanceData.ModuleFieldMap;
                if (moduleFieldMap.find(component.ModuleName) != moduleFieldMap.end()) {
                    auto& publicFields = moduleFieldMap.at(component.ModuleName);
                    for (auto& [name, field] : publicFields) {
                        bool isRuntime = m_SceneContext->m_IsPlaying && field.IsRuntimeAvailable();
                        switch (field.Type) {
                        case Script::FieldType::Bool: {
                            bool value = isRuntime ? field.GetRuntimeValue<bool>() : field.GetStoredValue<bool>();
                            if (UI::DrawBoolControl(field.Name.c_str(), &value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Char: {
                            char value = isRuntime ? field.GetRuntimeValue<char>() : field.GetStoredValue<char>();
                            if (UI::DrawCharControl(field.Name.c_str(), &value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Int: {
                            int value = isRuntime ? field.GetRuntimeValue<int>() : field.GetStoredValue<int>();
                            if (UI::DrawIntControl(field.Name.c_str(), &value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Float: {
                            float value = isRuntime ? field.GetRuntimeValue<float>() : field.GetStoredValue<float>();
                            if (UI::DrawFloatControl(field.Name.c_str(), &value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Vec2: {
                            glm::vec2 value = isRuntime ? field.GetRuntimeValue<glm::vec2>() : field.GetStoredValue<glm::vec2>();
                            if (UI::DrawFloat2Control(field.Name.c_str(), value, 0.2f)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Vec3: {
                            glm::vec3 value = isRuntime ? field.GetRuntimeValue<glm::vec3>() : field.GetStoredValue<glm::vec3>();
                            if (UI::DrawFloat3Control(field.Name.c_str(), value, 0.2f)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Vec4: {
                            glm::vec4 value = isRuntime ? field.GetRuntimeValue<glm::vec4>() : field.GetStoredValue<glm::vec4>();
                            if (UI::DrawFloat4Control(field.Name.c_str(), value, 0.2f)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        }
                    }
                }
            }
            UI::EndGrid();
        });

        DrawComponent<RigidBody2DComponent>("RigidBody", entity, [](auto& component) {
            auto rigidBody = component.RigidBody;
            
            glm::vec2 position = rigidBody.GetPosition();
            float rotation = rigidBody.GetRotation();
            ImGui::InputFloat2("Position", glm::value_ptr(position), 2);
            ImGui::InputFloat("Rotation", &rotation);



            //rigidBody.SetTransform(position, rotation);

            float friction = rigidBody.GetFriction();
            ImGui::InputFloat("Friction Value", &friction);
            rigidBody.SetFriction(friction);

            float density = rigidBody.GetDensity();
            ImGui::InputFloat("Density Value", &density);
            rigidBody.SetDensity(density);

            uint32_t dynamic = rigidBody.GetType();
            const std::array<const char*, 3> types = { "Static", "Kinematic", "Dynamic" };
            ImGui::Combo("RigidBody Type", (int*)&dynamic, types.data(), types.size());
            rigidBody.SetType((RigidBodyType)dynamic);
        });
        /*
        DrawComponent<Box2DShapeComponent>("Box2DShape", entity, [](auto& component) {
            //float 
           // ImGui::InputFloat("Density", &component.FixtureDef->density);
        });
        */
        if (removeEntity) {
            m_SceneContext->DestroyEntity(m_SelectionContext);
            m_SelectionContext = {};
        }
    }

    void SceneHierarchyPanel::DrawEnvironment() {
        ImGui::DragFloat3("Light Direction", glm::value_ptr(m_SceneContext->GetLight().Direction), 0.01, -10.0f, 10.0f);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_SceneContext->GetLight().Radiance));
        float& lightMulti = m_SceneContext->GetLight().Multiplier;
        ImGui::SliderFloat("Light Multiplier", &lightMulti, 0.0, 2.0);
    }
}