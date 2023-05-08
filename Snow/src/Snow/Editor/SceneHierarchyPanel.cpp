#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Snow/Scene/Components.h"

#include "Snow/Script/ScriptEngine.h"
#include "Snow/Render/Renderer.h"
#include "Snow/Render/MeshFactory.h"

#include <glm/gtc/type_ptr.hpp>

#include "Snow/Utils/FileDialogs.h"

#include "Snow/ImGui/ImGui.h"
#include "Snow/ImGui/ImGuiUtilities.h"

#include "Snow/Asset/AssetManager.h"

#include <limits>

namespace Snow {
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
        //m_CheckerboardTexture = Render::Texture2D::Create("assets/textures/Checkerboard.png");

        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) {
        m_SceneContext = scene;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender(bool window) {
        
        if (window) {
            ImGui::Begin("Scene Hierarchy");
        }

        if (m_SceneContext) {
            
            uint32_t entityCount = 0;

            m_SceneContext->m_Registry.each([&](auto entity) {
                Entity e{ entity, m_SceneContext.Raw() };
                if(e.HasComponent<IDComponent>())
                    DrawEntityNode(e);
            });

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                m_SelectionContext = {};

            if (ImGui::BeginPopupContextWindow(0, ImGuiMouseButton_Right, false)) {
                if (ImGui::MenuItem("Create empty entity")) {
                    m_SelectionContext = m_SceneContext->CreateEntity("Empty Entity");
                }
                if (ImGui::BeginMenu("2D Objects")) {
                    if (ImGui::MenuItem("Sprite")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Sprite");

                        auto& spriteComp = m_SelectionContext.AddComponent<SpriteRendererComponent>();
                        spriteComp.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
                        //spriteComp.Texture = AssetManager::CreateMemoryOnlyAsset<Texture>(Renderer::GetWhiteTexture());
                    }

                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("3D Objects")) {
                    if (ImGui::MenuItem("Plane")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Plane");
                        //Ref<Render::Mesh> mesh = ;
                        auto meshAsset = AssetManager::CreateMemoryOnlyAsset<Mesh>(Render::MeshFactory::CreateSquare(1.0f, 1.0f));
                        // = mesh->Handle;
                        //Ref<Render::Mesh> mesh = Ref<Render::Mesh>::Create(Ref<MeshAsset>::Create("assets/models/Plane.gltf"));
                        auto& meshComp = m_SelectionContext.AddComponent<MeshComponent>(meshAsset);
                        meshComp.MaterialTable = Ref<MaterialTable>::Create(1);
                        auto material = Render::Material::Create(Render::Renderer::GetShaderLibrary()->Get("PBR"));
                        meshComp.MaterialTable->SetMaterial(0, Ref<MaterialAsset>::Create(material));
                    }
                    if (ImGui::MenuItem("Cube")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Cube");
                        auto meshAsset = AssetManager::CreateMemoryOnlyAsset<Mesh>(Render::MeshFactory::CreateCube({ 1.0f, 1.0f, 1.0f }));
                        auto& meshComp = m_SelectionContext.AddComponent<MeshComponent>(meshAsset);
                        meshComp.MaterialTable = Ref<MaterialTable>::Create(1);
                        auto material = Render::Material::Create(Render::Renderer::GetShaderLibrary()->Get("PBR"));
                        meshComp.MaterialTable->SetMaterial(0, Ref<MaterialAsset>::Create(material));
                    }
                    if (ImGui::MenuItem("Sphere")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Sphere");
                        auto meshAsset = AssetManager::CreateMemoryOnlyAsset<Mesh>(Render::MeshFactory::CreateSphere(1.0f, 16.0f));
                        auto& meshComp = m_SelectionContext.AddComponent<MeshComponent>(meshAsset);
                        meshComp.MaterialTable = Ref<MaterialTable>::Create(1);
                        auto material = Render::Material::Create(Render::Renderer::GetShaderLibrary()->Get("PBR"));
                        meshComp.MaterialTable->SetMaterial(0, Ref<MaterialAsset>::Create(material));
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Lights")) {
                    if (ImGui::MenuItem("Directional Light")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Directional Light");
                        auto& directLightComp = m_SelectionContext.AddComponent<DirectionalLightComponent>();
                    }
                    if (ImGui::MenuItem("Point Light")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Point Light");
                        auto& pointLoghtComp = m_SelectionContext.AddComponent<PointLightComponent>();
                    }
                    if (ImGui::MenuItem("Sky Light")) {
                        m_SelectionContext = m_SceneContext->CreateEntity("Sky Light");
                        auto& skyLightComp = m_SelectionContext.AddComponent<SkyLightComponent>();
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }

            ImGui::End();

            ImGui::Begin("Properties");
            if (m_SelectionContext)
                DrawComponents(m_SelectionContext);

            ImGui::End();

            DrawEnvironment();
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

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem(UI::GenerateID())) {
            if (ImGui::MenuItem("Delete"))
                entityDeleted = true;

            ImGui::EndPopup();
        }

        if (entityDeleted) {
            m_SceneContext->DestroyEntity(m_SelectionContext);
            if(entity == m_SelectionContext)
                m_SelectionContext = {};
        }
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool canBeRemoved = true) {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>()) {
            ImGui::PushID((void*)typeid(T).hash_code());

            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();



            UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();

            bool opened = ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(T).hash_code()), treeNodeFlags, name.c_str());

            UI::ScopedColorStack button(ImGuiCol_Button, ImVec4(0, 0, 0, 0), ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup("ComponentSettings");
            }


            bool resetValues = false;
            bool removeComponent = false;
            if (UI::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Reset"))
                    resetValues = true;

                if (canBeRemoved) {
                    if (ImGui::MenuItem("Remove component"))
                        removeComponent = true;
                }

                UI::EndPopup();
            }

            if (opened) {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent || resetValues)
                entity.RemoveComponent<T>();

            if (resetValues)
                entity.AddComponent<T>();

            //if(!open)


            ImGui::PopID();
        }
    }

    static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
        bool modified = false;

        UI::PushID();

        ImGui::Text(label.c_str());

        {
            const float spacingX = 8.0f;

            UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2{ spacingX, 0.0f });
            UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });

            {
                UI::ScopedColor padding(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
                UI::ScopedStyle frame(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));

                ImGui::BeginChild(ImGui::GetID((label + "fr").c_str()), ImVec2(ImGui::GetContentRegionAvail().x - spacingX,
                    ImGui::GetFrameHeightWithSpacing() + 8.0f), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            }

            const float framePadding = 2.0f;
            const float outlineSpacing = 1.0f;
            const float lineHeight = GImGui->Font->FontSize + framePadding * 2.0f;
            const ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
            const float inputItemWidth = (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;

            const ImGuiIO& io = ImGui::GetIO();
            auto boldFont = io.Fonts->Fonts[0];

            auto drawControl = [&](const std::string& label, float& value, const ImVec4& colorN, const ImVec4& colorH, const ImVec4& colorP) {

                {
                    UI::ScopedStyle buttonFrame(ImGuiStyleVar_FramePadding, ImVec2(framePadding, 0.0f));
                    UI::ScopedStyle buttonRounding(ImGuiStyleVar_FrameRounding, 1.0f);
                    UI::ScopedColorStack buttonColors(ImGuiCol_Button, colorN, ImGuiCol_ButtonHovered, colorH, ImGuiCol_ButtonActive, colorP);

                    UI::ScopedFont buttonFont(boldFont);

                    if (ImGui::Button(label.c_str(), buttonSize)) {
                        value = resetValue;
                        modified = true;
                    }
                }

                ImGui::SameLine(0.0f, outlineSpacing);
                ImGui::SetNextItemWidth(inputItemWidth);

                modified |= ImGui::DragFloat(("##" + label).c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
            };

            drawControl("X", values.x, ImVec4{ 0.8f, 0.15f, 0.1f, 1.0f }, ImVec4{ 0.9f, 0.25f, 0.15f, 1.0f }, ImVec4{ 0.8f, 0.15f, 0.1f, 1.0f });
            
            ImGui::SameLine(0.0, outlineSpacing);
            drawControl("Y", values.y, ImVec4{ 0.2f, 0.8f, 0.1f, 1.0f }, ImVec4{ 0.25f, 0.85f, 0.15f, 1.0f }, ImVec4{ 0.2f, 0.8f, 0.1f, 1.0f });

            ImGui::SameLine(0.0, outlineSpacing);
            drawControl("Z", values.z, ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f }, ImVec4{ 0.2f, 0.25f, 0.85f, 1.0f }, ImVec4{ 0.1f, 0.2f, 0.8f, 1.0f });

            ImGui::EndChild();
        }
        UI::PopID();
        return modified;
    }

    template<typename Component>
    void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& name) {
        if (!m_SelectionContext.HasComponent<Component>()) {
            if (ImGui::MenuItem(name.c_str())) {
                m_SelectionContext.AddComponent<Component>();
                ImGui::CloseCurrentPopup();
            }
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity) {

        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        auto id = entity.GetComponent<IDComponent>().ID;

        bool isHovering = false;

        if (entity.HasComponent<TagComponent>()) {


            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            ImGui::PushItemWidth(contentRegionAvailable.x * 0.5f);

            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                tag = std::string(buffer);

            isHovering = ImGui::IsItemHovered();

            ImGui::PopItemWidth();
        }

        if (isHovering) {
            ImGui::SameLine();
            ImGui::TextDisabled("%llx", id);
        }
        
        //bool removeEntity = false;
       // if (ImGui::Button("Delete Entity"))
         //   removeEntity = true;


        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 textSize = ImGui::CalcTextSize("Add Component");
        textSize.x += GImGui->Style.FramePadding.x * 2.0f;

        {
            ImGui::SameLine(contentRegionAvailable.x - (textSize.x + GImGui->Style.FramePadding.x));
            if (ImGui::Button("Add Component", ImVec2(textSize.x + 4.0f, lineHeight + 2.0f)))
                ImGui::OpenPopup("AddComponentPanel");
        }

        {
            if (UI::BeginPopup("AddComponentPanel")) {
                DisplayAddComponentEntry<CameraComponent>("Camera");
                DisplayAddComponentEntry<MeshComponent>("Mesh");
                DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
                DisplayAddComponentEntry<ScriptComponent>("Script");
                DisplayAddComponentEntry<RigidBody2DComponent>("Rigidbody 2D");
                DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
                DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

                UI::EndPopup();
                /*
                if (ImGui::MenuItem("BRDF Material")) {

                    Ref<Render::Shader> shader = Render::Renderer::GetShaderLibrary()->Get("PBR");
                    Ref<Render::Material> mat = Render::Material::Create(shader);
                    //Ref<Render::MaterialInstance> m_MaterialInstance = Ref<Render::MaterialInstance>::Create(mat);
                    //m_SelectionContext.AddComponent<BRDFMaterialComponent>(m_MaterialInstance);

                    ImGui::CloseCurrentPopup();
                }
                */
            }
        }

        DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {
            UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
            UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

            //ImGui::BeginTable("transformComponent", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoClip);
            DrawVec3Control("Translation", component.Translation);

            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);

            DrawVec3Control("Scale", component.Scale, 1.0f);


            //component.UpdateTransform();
        }, false);

        DrawComponent<MeshComponent>("Mesh", entity, [&](MeshComponent& mc) {
            //UI::BeginPropertyGrid();
            //if (mc.Mesh && mc.Mesh->IsValid()) {
            //    if (UI::Button("Change Mesh", ImVec2(128, 128))) {
             //       auto file = Snow::Utils::FileDialogs::OpenFile().value();
            //        if (!file.empty()) {
            //            SNOW_CORE_TRACE("Congrats");
                        //auto mesh = MeshAsset(file);
                        //mc.Mesh = Ref<Mesh>::Create(mesh);
                        //auto materials = mesh.GetMaterials();
                        //for (int i = 0; i < materials.size(); i++)
                        //    mc.MaterialTable->SetMaterial(i, materials[i]);
           //         }

            //    }
           // }

            if (AssetManager::IsAssetHandleValid(mc.Mesh)) {
                auto mesh = AssetManager::GetAsset<Mesh>(mc.Mesh);
                if (mesh && mesh->IsValid()) {
                    if (UI::BeginTreeNode("Materials")) {
                        //UI::BeginPropertyGrid();

                        auto& meshMaterialTable = mesh->GetMaterials();
                        if (mc.MaterialTable->GetMaterialCount() < meshMaterialTable->GetMaterialCount())
                            mc.MaterialTable->SetMaterialCount(meshMaterialTable->GetMaterialCount());

                        auto playback = [](const Ref<Render::Texture2D>& icon, const float maxSize, const ImColor& tint) {
                            const float size = std::min((float)icon->GetHeight(), maxSize);
                            const float iconPadding = 2.0f;
                            const bool clicked = ImGui::InvisibleButton(UI::GenerateID(), ImVec2(size, size));
                            //if (index)
                            UI::DrawButtonImage(icon, tint, UI::ColorWithMultipliedValue(tint, 1.3f), UI::ColorWithMultipliedValue(tint, 0.7f), UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));

                            return clicked;
                        };

                        for (size_t i = 0; i < mc.MaterialTable->GetMaterialCount(); i++) {
                            if (i == meshMaterialTable->GetMaterialCount())
                                ImGui::Separator();

                            bool hasLocalMaterial = mc.MaterialTable->HasMaterial(i);
                            bool hasMeshMaterial = meshMaterialTable->HasMaterial(i);

                            Ref<MaterialAsset> meshMaterialAsset;
                            if (hasMeshMaterial)
                                meshMaterialAsset = meshMaterialTable->GetMaterial(i);

                            Ref<MaterialAsset> materialAsset = hasLocalMaterial ? mc.MaterialTable->GetMaterial(i) : meshMaterialAsset;
                            //Ref<MaterialAsset> materialAsset = meshMaterialAsset;

                            std::string label = fmt::format("[Material {0}]", i);



                            for (auto [name, resource] : materialAsset->GetMaterial()->GetTextures()) {
                                switch (resource.first) { // Resource Type
                                case ResourceType::Texture2D: {
                                    auto texture = resource.second.As<Render::Texture2D>();
                                    if (playback(texture, 64, IM_COL32(255, 255, 255, 255))) {
                                        auto filePath = Snow::Utils::FileDialogs::OpenFile();
                                        if (filePath.has_value()) {
                                            Ref<Render::Texture2D> newTexture = Render::Texture2D::Create(filePath.value());
                                            materialAsset->GetMaterial()->Set(name, newTexture);
                                        }

                                    }
                                    ImGui::SameLine();
                                    ImGui::Text(name.c_str());
                                }

                                }
                            }

                            for (auto [name, type] : materialAsset->GetMaterial()->GetVariables()) {
                                using namespace Render;
                                switch (type.Type) {
                                case UniformType::Bool: {
                                    bool value = *materialAsset->GetMaterial()->GetVariables().at(name).Data.As<bool>();
                                    if (UI::Property(name.c_str(), value))
                                        materialAsset->GetMaterial()->Set(name, value);
                                    break;
                                }
                                case UniformType::Int: {
                                    int value = *materialAsset->GetMaterial()->GetVariables().at(name).Data.As<int>();
                                    if (UI::Property(name.c_str(), value))
                                        materialAsset->GetMaterial()->Set(name, value);
                                    break;
                                }
                                case UniformType::Float: {
                                    float value = *materialAsset->GetMaterial()->GetVariables().at(name).Data.As<float>();
                                    if (UI::Property(name.c_str(), value))
                                        materialAsset->GetMaterial()->Set(name, value);
                                    break;
                                }
                                case UniformType::Float2: {
                                    glm::vec2 value = *materialAsset->GetMaterial()->GetVariables().at(name).Data.As<glm::vec2>();
                                    if (UI::Property(name.c_str(), value))
                                        materialAsset->GetMaterial()->Set(name, value);
                                    break;
                                }
                                case UniformType::Float3: {
                                    glm::vec3 value = *materialAsset->GetMaterial()->GetVariables().at(name).Data.As<glm::vec3>();
                                    if (UI::Property(name.c_str(), value))
                                        materialAsset->GetMaterial()->Set(name, value);
                                    break;
                                }
                                case UniformType::Float4: {
                                    glm::vec4 value = *materialAsset->GetMaterial()->GetVariables().at(name).Data.As<glm::vec4>();
                                    if (UI::Property(name.c_str(), value))
                                        materialAsset->GetMaterial()->Set(name, value);
                                    break;
                                }
                                }
                            }

                            UI::PropertyAssetReferenceSettings settings;
                            if (hasLocalMaterial || !hasMeshMaterial) {
                                if (hasLocalMaterial) {
                                    settings.AdvanceToNextColumn = false;
                                    settings.WidthOffset = ImGui::GetStyle().ItemSpacing.x + 28.0f;
                                }
                                UI::PropertyAssetReferenceTarget<MaterialAsset>(label.c_str(), nullptr, materialAsset, [i, materialTable = mc.MaterialTable](Ref<MaterialAsset> materialAsset) mutable {
                                    materialTable->SetMaterial(i, materialAsset);
                                }, settings);

                            }
                            else {
                                std::string meshMaterialName = meshMaterialAsset->GetMaterial()->GetName();
                                if (meshMaterialName.empty())
                                    meshMaterialName = "Unnamed Material";

                                UI::PropertyAssetReferenceTarget<MaterialAsset>(label.c_str(), meshMaterialName.c_str(), materialAsset, [i, materialTable = mc.MaterialTable](Ref<MaterialAsset> materialAsset) mutable {
                                    materialTable->SetMaterial(i, materialAsset);
                                }, settings);
                            }

                            if (hasLocalMaterial) {
                                ImGui::SameLine();
                                float prevItemHeight = ImGui::GetItemRectSize().y;
                                if (ImGui::Button("X", { prevItemHeight, prevItemHeight }))
                                    mc.MaterialTable->ClearMaterial(i);
                                ImGui::NextColumn();
                            }
                        }

                        //const int materialIndex = 0;
                       // if (ImGui::Button("AddMaterial")) {
                        //    Ref<Render::Material> matInstance = Render::Material::Create(Render::Renderer::GetShaderLibrary()->Get("PBR"), "Default Material");
                        //    mc.MaterialTable->SetMaterial(materialIndex, matInstance);
                        //}


                        //UI::EndPropertyGrid();
                        UI::EndTreeNode();
                    }
                }
            }
            //UI::EndPropertyGrid();

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

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [this](SpriteRendererComponent& sprite) {
            UI::PropertyColor("Color", sprite.Color);
            //ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

            //Ref<Render::Texture2D> sprite = sprite.Texture;
            //void* textureID = sprite.Raw() == nullptr ? (void*)m_CheckerboardTexture->GetRendererID() : (void*)sprite->GetRendererID();

            //ImGui::Image(textureID, ImVec2(64, 64), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
            //if (ImGui::IsItemClicked()) {
                //std::optional<std::string> filepath = Utils::FileDialogs::OpenFile("");
                //if (filepath.has_value()) {
                //    component.Texture = Render::Texture2D::Create(filepath.value());
               // }
           // }
            
        });

        

        DrawComponent<BRDFMaterialComponent>("Material", entity, [this](auto& component) {

            
            /*
            //Ref<Render::MaterialInstance> mat = component.MaterialInstance;

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
            */
        });

        DrawComponent<ScriptComponent>("Script", entity, [=](ScriptComponent& sc) mutable {
            UI::BeginPropertyGrid();

            bool isError = !Script::ScriptEngine::ModuleExists(sc.ModuleName);
            

            std::string name = sc.ModuleName;

            bool err = isError;
            if (err)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));



            if (UI::Property("Module Name", sc.ModuleName)) {

                if (!isError)
                    Script::ScriptEngine::ShutdownScriptEntity(entity, sc.ModuleName);

                if (Script::ScriptEngine::ModuleExists(name)) {
                    sc.ModuleName = name;
                    isError = false;
                }
                else if (false) {

                }
                else {
                    sc.ModuleName = name;
                    isError = true;
                }

                if (!isError)
                    Script::ScriptEngine::InitScriptEntity(entity);
            }
            if (err)
                ImGui::PopStyleColor();

            if (!isError) {
                Script::EntityInstanceData& entityInstanceData = Script::ScriptEngine::GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID());
                auto& moduleFieldMap = entityInstanceData.ModuleFieldMap;
                if (moduleFieldMap.find(sc.ModuleName) != moduleFieldMap.end()) {
                    auto& publicFields = moduleFieldMap.at(sc.ModuleName);
                    for (auto& [name, field] : publicFields) {
                        bool isRuntime = m_SceneContext->m_IsPlaying && entityInstanceData.Instance.IsRuntimeAvailable();

                        //if (field.IsReadOnly)
                        //    UI::PushItemDisabled();


                        switch (field.Type) {
                        case Script::FieldType::Bool: {
                            bool value = isRuntime ? field.GetRuntimeValue<bool>(entityInstanceData.Instance) : field.GetStoredValue<bool>();
                            if (UI::Property(field.Name.c_str(), value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Char: {
                            char value = isRuntime ? field.GetRuntimeValue<char>(entityInstanceData.Instance) : field.GetStoredValue<char>();
                            if (UI::Property(field.Name.c_str(), value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Int: {
                            int value = isRuntime ? field.GetRuntimeValue<int>(entityInstanceData.Instance) : field.GetStoredValue<int>();
                            if (UI::Property(field.Name.c_str(), value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Float: {
                            float value = isRuntime ? field.GetRuntimeValue<float>(entityInstanceData.Instance) : field.GetStoredValue<float>();
                            if (UI::Property(field.Name.c_str(), value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Double: {
                            double value = isRuntime ? field.GetRuntimeValue<double>(entityInstanceData.Instance) : field.GetStoredValue<float>();
                            if (UI::Property(field.Name.c_str(), *(float*)&value)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Vec2: {
                            glm::vec2 value = isRuntime ? field.GetRuntimeValue<glm::vec2>(entityInstanceData.Instance) : field.GetStoredValue<glm::vec2>();
                            if (UI::Property(field.Name.c_str(), value, 0.2f)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Vec3: {
                            glm::vec3 value = isRuntime ? field.GetRuntimeValue<glm::vec3>(entityInstanceData.Instance) : field.GetStoredValue<glm::vec3>();
                            if (UI::Property(field.Name.c_str(), value, 0.2f)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        case Script::FieldType::Vec4: {
                            glm::vec4 value = isRuntime ? field.GetRuntimeValue<glm::vec4>(entityInstanceData.Instance) : field.GetStoredValue<glm::vec4>();
                            if (UI::Property(field.Name.c_str(), value, 0.2f)) {
                                if (isRuntime)
                                    field.SetRuntimeValue(entityInstanceData.Instance, value);
                                else
                                    field.SetStoredValue(value);
                            }
                            break;
                        }
                        }
                    }
                }
            }
            UI::EndPropertyGrid();
        });

        DrawComponent<RigidBody2DComponent>("Rigid Body 2D", entity, [](RigidBody2DComponent& component) {

            const std::array<const char*, 3> types = { "Static", "Dynamic", "Kinematic" };
            const char* currentBodyType = types[(int)component.Type];
            if (ImGui::BeginCombo("Body Type", currentBodyType)) {
                for (int i = 0; i < 3; i++) {
                    bool isSelected = currentBodyType == types[i];
                    if (ImGui::Selectable(types[i], isSelected)) {
                        currentBodyType = types[i];
                        component.Type = (RigidBody2DComponent::BodyType)i;
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            UI::Property("Fixed Rotation", component.FixedRotation);

            
            
        });
        
        DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](BoxCollider2DComponent& component) {
           
            UI::Property("Offset", component.Offset);
            UI::Property("Size", component.Size);
            UI::Property("Density", component.Density);
            UI::Property("Friction", component.Friction);
            UI::Property("Restitution", component.Restitution);
            UI::Property("Restitution Threshold", component.RestitutionThreshold);
        });

        DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](CircleCollider2DComponent& component) {

            UI::Property("Radius", component.Radius);
            UI::Property("Offset", component.Offset);
            UI::Property("Density", component.Density);
            UI::Property("Friction", component.Friction);
            UI::Property("Restitution", component.Restitution);
            UI::Property("Restitution Threshold", component.RestitutionThreshold);
        });
        
        DrawComponent<DirectionalLightComponent>("Directional Light", entity, [](DirectionalLightComponent& component) {
            UI::BeginPropertyGrid();
            UI::PropertyColor("Light Radiance", component.Radiance);
            UI::PropertySlider("Light Direction", component.Direction, -1.0f, 1.0f);
            UI::Property("Light Intensity", component.Intensity);
            UI::EndPropertyGrid();
        });

        DrawComponent<SkyLightComponent>("Sky Light", entity, [](SkyLightComponent& component) {
            UI::BeginPropertyGrid();
            UI::Property("Intensity", component.Intensity, 0.01f, 0.0f, 5.0f);
            
            if (AssetManager::IsAssetHandleValid(component.SceneEnvironment)) {
                auto env = AssetManager::GetAsset<Environment>(component.SceneEnvironment);
                if (env && env->RadianceMap) {
                    UI::PropertySlider("Lod", component.Lod, 0.0f, env->RadianceMap->GetMipLevelCount());

                }
                else {
                    UI::PropertySlider("Lod", component.Lod, 0.0f, 10.0f);
                }
            }

            UI::Property("Dynamic Sky", component.DynamicSky);
            if (component.DynamicSky) {
                bool changed = UI::Property("Trubidity", component.TurbidityAzimuthInclination.x, 0.01);
                changed |= UI::Property("Azimuth", component.TurbidityAzimuthInclination.y, 0.01);
                changed |= UI::Property("Inclination", component.TurbidityAzimuthInclination.z, 0.01);
                if (changed) {
                    if (AssetManager::IsMemoryAsset(component.SceneEnvironment)) {
                        Ref<TextureCube> preethEnv = Renderer::CreatePreethamSky(component.TurbidityAzimuthInclination.x, component.TurbidityAzimuthInclination.y, component.TurbidityAzimuthInclination.z);
                        Ref<Environment> environment = AssetManager::GetAsset<Environment>(component.SceneEnvironment);
                        if (environment) {
                            environment->RadianceMap = preethEnv;
                            environment->IrradianceMap = preethEnv;
                        }
                    }
                    else {
                        Ref<TextureCube> preethEnv = Renderer::CreatePreethamSky(component.TurbidityAzimuthInclination.x, component.TurbidityAzimuthInclination.y, component.TurbidityAzimuthInclination.z);
                        component.SceneEnvironment = AssetManager::CreateMemoryOnlyAsset<Environment>(preethEnv, preethEnv);
                    }
                }
            }
            UI::EndPropertyGrid();
        });
    }

    void SceneHierarchyPanel::DrawEnvironment() {
        auto* light = &m_SceneContext->m_LightEnvironment.DirectionalLights[0];
        ImGui::Begin("Scene Environment");
        UI::BeginPropertyGrid();
        UI::PropertySlider("Light Direction", light->Direction, -1.0f, 1.0f);
        UI::PropertyColor("Light Color", light->Radiance);
        UI::Property("Light Intensity", light->Multiplier, 0.0f, 10.0f);
        UI::EndPropertyGrid();
        ImGui::End();
        //ImGui::DragFloat3("Light Direction", glm::value_ptr(.Direction), 0.01f, -10.0f, 10.0f);
        //ImGui::ColorEdit3("Light Color", glm::value_ptr(m_SceneContext->GetLight().Radiance));
        //float& lightMulti = m_SceneContext->GetLight().Multiplier;
        //ImGui::SliderFloat("Light Multiplier", &lightMulti, 0.0, 2.0);
    }
}