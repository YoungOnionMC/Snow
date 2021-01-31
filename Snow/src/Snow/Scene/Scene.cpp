#include <spch.h>
#include "Snow/Scene/Scene.h"

#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/Scene/Components.h"
#include "Snow/Scene/Entity.h"

#include "Snow/Math/Mat4.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Snow/Render/SceneRenderer.h"
#include "Snow/Script/ScriptEngine.h"

namespace Snow {

    std::unordered_map<UUID, Scene*> s_ActiveScenes;

    struct SceneComponent {
        UUID SceneID;
    };

    static void OnScriptComponentConstruct(entt::registry& registry, entt::entity entity) {
        auto sceneView = registry.view<SceneComponent>();
        UUID sceneID = registry.get<SceneComponent>(sceneView.front()).SceneID;

        Scene* scene = s_ActiveScenes[sceneID];

        auto entityID = registry.get<IDComponent>(entity).ID;
        SNOW_CORE_ASSERT(scene->m_EntityIDMap.find(entityID) != scene->m_EntityIDMap.end());
        Script::ScriptEngine::InitScriptEntity(scene->m_EntityIDMap.at(entityID));
    }

    static void OnScriptComponentDestroy(entt::registry& registry, entt::entity entity) {
        auto sceneView = registry.view<SceneComponent>();
        UUID sceneID = registry.get<SceneComponent>(sceneView.front()).SceneID;

        Scene* scene = s_ActiveScenes[sceneID];

        auto entityID = registry.get<IDComponent>(entity).ID;
        Script::ScriptEngine::OnScriptComponentDestroyed(sceneID, entityID);
    }

    Scene::Scene(const std::string& name) :
        m_Name(name) {

        m_Registry.on_construct<ScriptComponent>().connect<&OnScriptComponentConstruct>();
        m_Registry.on_destroy<ScriptComponent>().connect<&OnScriptComponentDestroy>();

        m_SceneEntity = m_Registry.create();
        m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID);

        m_Light.Direction = glm::vec3(0.1, 0.0, 1.0);
        m_Light.Radiance = glm::vec3(1.0, 1.0, 1.0);
        m_PhysicsWorld = new b2World(m_Gravity);

        s_ActiveScenes[m_SceneID] = this;


        m_EnvMap = Render::API::TextureCube::Create("assets/textures/SunnyTextureCube.png");
        m_BRDFLUT = Render::API::Texture2D::Create("assets/textures/BRDFLUT.png");
    }

    Scene::~Scene() {
        m_Registry.on_destroy<ScriptComponent>().disconnect();

        m_Registry.clear();
        s_ActiveScenes.erase(m_SceneID);
        Script::ScriptEngine::OnSceneDestruct(m_SceneID);
    }

    Entity Scene::CreateEntity(const std::string& name) {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = {};

        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;

        m_EntityIDMap[idComponent.ID] = entity;
        return entity;
    }

    Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name, bool runtimeMap) {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = uuid;

        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;

        SNOW_CORE_ASSERT(m_EntityIDMap.find(uuid) == m_EntityIDMap.end());
        m_EntityIDMap[uuid] = entity;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Registry.destroy(entity);
    }

    void Scene::OnRuntimeStart() {
        Script::ScriptEngine::SetSceneContext(this);
        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view) {
                Entity e = { entity, this };
                if (Script::ScriptEngine::ModuleExists(e.GetComponent<ScriptComponent>().ModuleName))
                    Script::ScriptEngine::InstantiateEntityClass(e);
            }
        }

        m_IsPlaying = true;
    }

    void Scene::OnRuntimeStop() {
        m_IsPlaying = false;
    }

    void Scene::OnUpdate(Timestep ts) {

        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
                if (!nsc.Instance) {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = Entity{ entity, this };
                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate(ts);

            });
        }

        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view) {
                UUID entityID = m_Registry.get<IDComponent>(entity).ID;
                Entity e = { entity, this };
                if (Script::ScriptEngine::ModuleExists(e.GetComponent<ScriptComponent>().ModuleName))
                    Script::ScriptEngine::OnUpdateEntity(m_SceneID, entityID, ts);
            }
        }

        m_PhysicsWorld->Step(ts, 6, 2);
        {

            auto view = m_Registry.view<RigidBody2DComponent>();
            for (auto entity : view) {
                Entity e = { entity, this };
                auto& transform = e.GetTransform();
                auto rigidBody2D = e.GetComponent<RigidBody2DComponent>();

                auto& position = rigidBody2D.RigidBody->GetPosition();
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, transform[3].z }) *
                    glm::toMat4(glm::quat({ rotation.x, rotation.y, rigidBody2D.RigidBody->GetAngle() })) *
                    glm::scale(glm::mat4(1.0f), scale);
            }
        }
    }

    void Scene::OnRenderRuntime(Timestep ts) {
        Entity cameraEntity = GetMainCamera();
        SNOW_CORE_ASSERT(cameraEntity.m_Scene);
        if (!cameraEntity || !cameraEntity.m_Scene)
            return;

        SNOW_CORE_ASSERT(cameraEntity.HasComponent<TransformComponent>() && cameraEntity.HasComponent<CameraComponent>(), "Scene does not contain any cameras!");

        glm::mat4 cameraViewMatrix = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);

        Render::SceneRenderer::BeginScene(this, { camera, cameraViewMatrix });
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent, BRDFMaterialComponent>();
            for (auto entity : group) {
                auto [transform, mesh, material] = group.get<TransformComponent, MeshComponent, BRDFMaterialComponent>(entity);

                if (mesh.Mesh.Raw() != nullptr) {
                    //auto material = group.get<BRDFMaterialComponent>(entity);

                    if (material.MaterialInstance) {
                        auto& matInstance = material.MaterialInstance;
                        matInstance->Set("AlbedoColor", material.AlbedoInput.AlbedoColor);
                        matInstance->Set("Metalness", material.MetalnessInput.Value);
                        matInstance->Set("Roughness", material.RoughnessInput.Value);

                        matInstance->Set("RadiancePrefilter", 0.0f);

                        matInstance->Set("AlbedoTexToggle", material.AlbedoInput.UseTexture ? 1.0f : 0.0f);
                        if (material.AlbedoInput.UseTexture)
                            matInstance->Set("u_AlbedoTexture", material.AlbedoInput.AlbedoTexture);

                        matInstance->Set("NormalTexToggle", material.NormalInput.UseTexture ? 1.0f : 0.0f);
                        if (material.NormalInput.UseTexture)
                            matInstance->Set("u_NormalTexture", material.NormalInput.NormalTexture);

                        matInstance->Set("MetalnessTexToggle", material.MetalnessInput.UseTexture ? 1.0f : 0.0f);
                        if (material.MetalnessInput.UseTexture)
                            matInstance->Set("u_MetalnessTexture", material.MetalnessInput.MetalnessTexture);

                        matInstance->Set("RoughnessTexToggle", material.RoughnessInput.UseTexture ? 1.0f : 0.0f);
                        if (material.RoughnessInput.UseTexture)
                            matInstance->Set("u_MetalnessTexture", material.RoughnessInput.RoughnessTexture);

                        matInstance->Set("u_EnvRadianceTexture", m_EnvMap);
                    }

                    Render::SceneRenderer::SubmitMesh(mesh.Mesh, transform.GetTransform(), material.MaterialInstance);
                }
            }
        }
        {
            auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                if (!sprite.Texture)
                    Render::SceneRenderer::Submit2DQuad(transform.GetTransform(), sprite.Color);
                else
                    Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color);
            }
        }
        Render::SceneRenderer::EndScene();
          
        
    }

    void Scene::OnRenderEditor(Timestep ts, Render::EditorCamera& editorCamera) {
        Render::SceneRenderer::BeginScene(this, editorCamera);
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent, BRDFMaterialComponent>();
            for (auto entity : group) {
                auto [transform, mesh, material] = group.get<TransformComponent, MeshComponent, BRDFMaterialComponent>(entity);

                if (mesh.Mesh.Raw() != nullptr) {
                    //auto material = group.get<BRDFMaterialComponent>(entity);

                    if (material.MaterialInstance) {
                        auto& matInstance = material.MaterialInstance;
                        matInstance->Set("AlbedoColor", material.AlbedoInput.AlbedoColor);
                        matInstance->Set("Metalness", material.MetalnessInput.Value);
                        matInstance->Set("Roughness", material.RoughnessInput.Value);

                        matInstance->Set("RadiancePrefilter", 0.0f);

                        matInstance->Set("AlbedoTexToggle", material.AlbedoInput.UseTexture ? 1.0f : 0.0f);
                        if (material.AlbedoInput.UseTexture)
                            matInstance->Set("u_AlbedoTexture", material.AlbedoInput.AlbedoTexture);

                        matInstance->Set("NormalTexToggle", material.NormalInput.UseTexture ? 1.0f : 0.0f);
                        if (material.NormalInput.UseTexture)
                            matInstance->Set("u_NormalTexture", material.NormalInput.NormalTexture);

                        matInstance->Set("MetalnessTexToggle", material.MetalnessInput.UseTexture ? 1.0f : 0.0f);
                        if (material.MetalnessInput.UseTexture)
                            matInstance->Set("u_MetalnessTexture", material.MetalnessInput.MetalnessTexture);

                        matInstance->Set("RoughnessTexToggle", material.RoughnessInput.UseTexture ? 1.0f : 0.0f);
                        if (material.RoughnessInput.UseTexture)
                            matInstance->Set("u_MetalnessTexture", material.RoughnessInput.RoughnessTexture);

                        matInstance->Set("u_EnvRadianceTexture", m_EnvMap);
                        matInstance->Set("u_EnvIrradianceTexture", m_EnvMap);
                        matInstance->Set("u_BRDFLUTTexture", m_BRDFLUT);
                    }

                    Render::SceneRenderer::SubmitMesh(mesh.Mesh, transform.GetTransform(), material.MaterialInstance);
                }
            }
        }
        
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            if (!sprite.Texture)
                Render::SceneRenderer::Submit2DQuad(transform.GetTransform(), sprite.Color);
            else
                Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color);
        }

        

        Render::SceneRenderer::EndScene();
        
        
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComp = view.get<CameraComponent>(entity);
            if (!cameraComp.FixedAspectRatio)
                cameraComp.Camera.SetViewportSize(width, height);
        }

        Render::SceneRenderer::OnViewportResize(width, height);
    }

    template<typename T>
    static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap) {
        auto comps = srcRegistry.view<T>();
        for (auto srcEntity : comps) {
            entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);

            auto& srcComp = srcRegistry.get<T>(srcEntity);
            auto& destComp = dstRegistry.emplace_or_replace<T>(destEntity, srcComp);
        }
    }

    Entity Scene::FindEntityByTag(const std::string& tag) {
        auto view = m_Registry.view<TagComponent>();
        for (auto entity : view) {
            const auto& name = view.get<TagComponent>(entity).Tag;
            if (name == tag)
                return Entity(entity, this);
        }

        return Entity{};
    }

    void Scene::CopyScene(Ref<Scene> scene) {

        std::unordered_map<UUID, entt::entity> enttMap;
        auto idComponents = m_Registry.view<IDComponent>();
        for (auto entity : idComponents) {
            auto uuid = m_Registry.get<IDComponent>(entity).ID;
            Entity e = scene->CreateEntityWithID(uuid, "", true);
            enttMap[uuid] = e.m_EntityHandle;
        }

        CopyComponent<TagComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<TransformComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<SpriteRendererComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<CameraComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<BRDFMaterialComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<MeshComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<ScriptComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<NativeScriptComponent>(scene->m_Registry, m_Registry, enttMap);
        CopyComponent<RigidBody2DComponent>(scene->m_Registry, m_Registry, enttMap);

        const auto& entityInstanceMap = Script::ScriptEngine::GetEntityInstanceMap();
        if (entityInstanceMap.find(scene->GetUUID()) != entityInstanceMap.end())
            Script::ScriptEngine::CopyEntityScriptData(scene->GetUUID(), m_SceneID);

        

    }

    Entity Scene::GetMainCamera() {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary) {
                return { entity, this };
            }
        }
        return Entity{};
    }

    Ref<Scene> Scene::GetScene(UUID uuid) {
        if (s_ActiveScenes.find(uuid) != s_ActiveScenes.end())
            return s_ActiveScenes.at(uuid);

        return {};
    }
}