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
        

        s_ActiveScenes[m_SceneID] = this;

        Init();
        //m_EnvMap = Render::API::TextureCube::Create("assets/textures/SunnyTextureCube.png");
        //m_BRDFLUT = Render::API::Texture2D::Create("assets/textures/BRDFLUT.png");
    }

    Scene::~Scene() {
        m_Registry.on_destroy<ScriptComponent>().disconnect();

        m_Registry.clear();
        s_ActiveScenes.erase(m_SceneID);
        Script::ScriptEngine::OnSceneDestruct(m_SceneID);
    }

    void Scene::Init() {
        m_SceneRenderer2D = Ref<Render::Renderer2D>::Create();
    }

    static b2BodyType Rigidbody2DTypeToBox2DBody(RigidBody2DComponent::BodyType bodyType) {
        switch (bodyType)
        {
        case RigidBody2DComponent::BodyType::Static:    return b2_staticBody;
        case RigidBody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
        case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }

        SNOW_CORE_ASSERT(false, "Unknown body type");
        return b2_staticBody;
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

            for (auto entity : view) {
                Entity e = { entity, this };
                if (Script::ScriptEngine::ModuleExists(e.GetComponent<ScriptComponent>().ModuleName))
                    Script::ScriptEngine::OnCreateEntity(e);
            }
        }

        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
        {
            auto view = m_Registry.view<RigidBody2DComponent>();
            m_Physics2DBodyEntityBuffer = new Entity[view.size()];
            uint32_t physicsbodyIndex = 0;
            for (auto e : view) {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

                b2BodyDef bodyDef;
                bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
                bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
                bodyDef.angle = transform.Rotation.z;

                
                //bodyDef.userData = (b2BodyUserData)(entityStorage);

                b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
                body->SetFixedRotation(rb2d.FixedRotation);
                Entity* entityStorage = &m_Physics2DBodyEntityBuffer[physicsbodyIndex++];
                *entityStorage = entity;
                body->GetUserData().pointer = (uintptr_t)entityStorage;
                rb2d.RuntimeBody = body;

            }
        }
        {
            auto view = m_Registry.view<BoxCollider2DComponent>();
            for (auto e : view) {
                Entity entity = { e, this };

                auto& transform = entity.GetComponent<TransformComponent>();
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

                if (entity.HasComponent<RigidBody2DComponent>()) {
                    SNOW_CORE_TRACE(entity.HasComponent<RigidBody2DComponent>());
                    auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

                    b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);

                    b2PolygonShape shape = b2PolygonShape();
                    shape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &shape;
                    fixtureDef.density = bc2d.Density;
                    fixtureDef.friction = bc2d.Friction;
                    fixtureDef.restitution = bc2d.Restitution;
                    fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
                    body->CreateFixture(&fixtureDef);
                }
            }
        }

        m_IsPlaying = true;
    }

    void Scene::OnRuntimeStop() {
        m_IsPlaying = false;

        delete[] m_Physics2DBodyEntityBuffer;
    }

    void Scene::OnUpdate(Timestep ts) {


        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view) {
                UUID entityID = m_Registry.get<IDComponent>(entity).ID;
                Entity e = { entity, this };
                if (Script::ScriptEngine::ModuleExists(e.GetComponent<ScriptComponent>().ModuleName))
                    Script::ScriptEngine::OnUpdateEntity(e, ts);
            }
        }

        m_PhysicsWorld->Step(ts, 6, 2);
        {

            auto view = m_Registry.view<RigidBody2DComponent>();
            for (auto entity : view) {
                Entity e = { entity, this };

                auto& rb2d = e.GetComponent<RigidBody2DComponent>();
                b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);

                auto& position = body->GetPosition();
                TransformComponent& transform = e.GetComponent<TransformComponent>();
                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();

                /*
                glm::mat4& transform = e.Transform();
                auto rigidBody2D = e.GetComponent<RigidBody2DComponent>();

                b2Body* body = (b2Body*)rigidBody2D.RuntimeBody;
                auto& position = body->GetPosition();
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);
                */
                transform.Transform = glm::translate(glm::mat4(1.0f), transform.Translation) *
                    glm::toMat4(glm::quat(transform.Rotation)) *
                    glm::scale(glm::mat4(1.0f), transform.Scale);
                SNOW_CORE_TRACE("Entity: {0}", e.GetComponent<TagComponent>().Tag.c_str());
                SNOW_CORE_TRACE("   Supposed Y: {0}", transform.Transform[3].y);
                SNOW_CORE_TRACE("   Box2D Y: {0}", position.y);
            }
        }

        
    }

    void Scene::OnRenderRuntime(Ref<Render::SceneRenderer> renderer, Timestep ts) {
        Entity cameraEntity = GetMainCamera();
        SNOW_CORE_ASSERT(cameraEntity.m_Scene, "No camera in the scene");
        if (!cameraEntity || !cameraEntity.m_Scene)
            return;

        SNOW_CORE_ASSERT(cameraEntity.HasComponent<TransformComponent>() && cameraEntity.HasComponent<CameraComponent>(), "Scene does not contain any cameras!");

        glm::mat4 cameraViewMatrix = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
        

        SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);

        glm::mat4 cameraViewProj = camera.GetProjection() * cameraViewMatrix;

        renderer->SetScene(this);
        renderer->BeginScene({ camera, cameraViewMatrix, 0.1f, 1000.0f, 45.0f });
        //Render::SceneRenderer::BeginScene( { camera, cameraViewMatrix });
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent, BRDFMaterialComponent>();
            for (auto entity : group) {
                auto [transform, mesh, material] = group.get<TransformComponent, MeshComponent, BRDFMaterialComponent>(entity);

                if (mesh.Mesh.Raw() != nullptr) {
                    //auto material = group.get<BRDFMaterialComponent>(entity);
                    /*
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
                            matInstance->Set("u_RoughnessTexture", material.RoughnessInput.RoughnessTexture);

                        matInstance->Set("u_EnvRadianceTexture", m_EnvMap);
                    }
                    */
                    //Render::SceneRenderer::SubmitMesh(mesh.Mesh, transform.GetTransform(), material.MaterialInstance);
                }
            }
        }
        renderer->EndScene();
        if (renderer->GetFinalPassImage()) {
            m_SceneRenderer2D->BeginScene(cameraViewProj, cameraViewMatrix);
            m_SceneRenderer2D->SetTargetRenderPass(renderer->GetCompositeRenderPass());
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                if (!sprite.Texture)
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), sprite.Color);
                else
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color);
            }

            m_SceneRenderer2D->EndScene();
        }
        //Render::SceneRenderer::EndScene();
          
        
    }

    void Scene::OnRenderEditor(Ref<Render::SceneRenderer> renderer, Timestep ts, Editor::EditorCamera& editorCamera) {
        //Render::SceneRenderer::BeginScene(editorCamera);
        renderer->SetScene(this);
        renderer->BeginScene({ editorCamera, editorCamera.GetViewMatrix(), 0.1f, 1000.0f, 45.0f });
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent>();
            for (auto entity : group) {
                auto [transform, meshComp] = group.get<TransformComponent, MeshComponent>(entity);

                if (false) {
                    auto mesh = meshComp.Mesh;
                    if (!mesh->IsFlagSet(AssetFlag::Missing)) {
                        Entity e = Entity(entity, this);

                        glm::mat4 transform = glm::mat4(1.0f);

                        renderer->SubmitMesh(mesh, meshComp.MaterialTable, transform);
                    }
                }
                    //auto material = group.get<BRDFMaterialComponent>(entity);
                    /*
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
                            matInstance->Set("u_RoughnessTexture", material.RoughnessInput.RoughnessTexture);

                        
                        matInstance->Set("u_EnvRadianceTexture", m_EnvMap);
                        matInstance->Set("u_EnvIrradianceTexture", m_EnvMap);
                        matInstance->Set("u_BRDFLUTTexture", m_BRDFLUT);
                        
                    }
                    */

                    //Render::SceneRenderer::SubmitMesh(mesh.Mesh, transform.GetTransform(), material.MaterialInstance);
                
            }
        }
        renderer->EndScene();

        if (renderer->GetFinalPassImage()) {
            m_SceneRenderer2D->BeginScene(editorCamera.GetViewProjectionMatrix(), editorCamera.GetViewMatrix());
            m_SceneRenderer2D->SetTargetRenderPass(renderer->GetCompositeRenderPass());
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                if (!sprite.Texture)
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), sprite.Color);
                else
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color);
            }

            m_SceneRenderer2D->EndScene();
        }
        //Render::SceneRenderer::EndScene();
        
        
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

        //Render::SceneRenderer::OnViewportResize(width, height);
    }

    template<typename Component>
    static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap) {
        auto view = srcRegistry.view<Component>();
        for (auto srcEntity : view) {
            entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);

            auto& srcComp = srcRegistry.get<Component>(srcEntity);
            auto& dstComp = dstRegistry.emplace_or_replace<Component>(destEntity, srcComp);
        }
    }

    template<typename Component>
    static void CopyComponentIfExists(Entity dst, Entity src) {
        if (src.HasComponent<Component>())
            dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
    }

    

    Ref<Scene> Scene::CopyScene(Ref<Scene> scene) {
        Ref<Scene> newScene = Ref<Scene>::Create("Testing Name");


        auto& srcRegistry = scene->m_Registry;
        auto& dstRegistry = newScene->m_Registry;

        std::unordered_map<UUID, entt::entity> enttMap;
        auto idComponents = srcRegistry.view<IDComponent>();

        for (auto entity : idComponents) {
            auto uuid = srcRegistry.get<IDComponent>(entity).ID;
            auto name = srcRegistry.get<TagComponent>(entity).Tag;
            Entity e = newScene->CreateEntityWithID(uuid, name, true);
            enttMap[uuid] = e.m_EntityHandle;
        }

        CopyComponent<TagComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<TransformComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<SpriteRendererComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<CameraComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<BRDFMaterialComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<MeshComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<ScriptComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<RigidBody2DComponent>(dstRegistry, srcRegistry, enttMap);
        CopyComponent<BoxCollider2DComponent>(dstRegistry, srcRegistry, enttMap);

        const auto& entityInstanceMap = Script::ScriptEngine::GetEntityInstanceMap();
        if (entityInstanceMap.find(scene->GetUUID()) != entityInstanceMap.end())
            Script::ScriptEngine::CopyEntityScriptData(scene->GetUUID(), scene->m_SceneID);

        
        return newScene;
    }

    Entity Scene::CreateEntity(const std::string& name) {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = {};

        entity.AddComponent<TransformComponent>();
        if (!name.empty())
            entity.AddComponent<TagComponent>(name);
        else
            entity.AddComponent<TagComponent>();

        m_EntityIDMap[idComponent.ID] = entity;
        return entity;
    }

    Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name, bool runtimeMap) {
        auto entity = Entity{ m_Registry.create(), this };
        auto& idComponent = entity.AddComponent<IDComponent>();
        idComponent.ID = uuid;

        entity.AddComponent<TransformComponent>();
        if (!name.empty())
            entity.AddComponent<TagComponent>(name);
        else
            entity.AddComponent<TagComponent>();

        SNOW_CORE_ASSERT(m_EntityIDMap.find(uuid) == m_EntityIDMap.end(), "Entity already part of the entity map");
        m_EntityIDMap[uuid] = entity;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {

        if (entity.HasComponent<ScriptComponent>())
            Script::ScriptEngine::OnScriptComponentDestroyed(m_SceneID, entity.GetUUID());

        m_Registry.destroy(entity);
    }

    Entity Scene::DuplicateEntity(Entity entity) {
        Entity newEntity;

        if (entity.HasComponent<TagComponent>())
            newEntity = CreateEntity(entity.GetComponent<TagComponent>().Tag);
        else
            newEntity = CreateEntity();

        CopyComponentIfExists<TransformComponent>(newEntity, entity);

        CopyComponentIfExists<MeshComponent>(newEntity, entity);
        CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
        CopyComponentIfExists<ScriptComponent>(newEntity, entity);
        CopyComponentIfExists<CameraComponent>(newEntity, entity);
        CopyComponentIfExists<RigidBody2DComponent>(newEntity, entity);
        CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);

        return newEntity;
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