#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Snow/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>

#include "Snow/Utils/FileDialogs.h"

namespace Snow {
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
//        m_CheckerboardTexture = Render::API::Texture2D::Create("assets/textures/Checkerboard.png");

        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) {
        m_SceneContext = scene;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");

        m_SceneContext->m_Registry.each([&](auto entityID) {
            Entity entity{ entityID, m_SceneContext.Raw() };
            DrawEntityNode(entity);
        });

        if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create empty entity"))
                m_SceneContext->CreateEntity("Empty Entity");

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if(m_SelectionContext)
            DrawComponents(m_SelectionContext);

        ImGui::End();

        ImGui::Begin("Environment");
        DrawEnvironment();
        ImGui::End();
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

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
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
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<T>();
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

            ImGui::EndPopup();

        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
            DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
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