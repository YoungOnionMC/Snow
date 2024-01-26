#include <spch.h>
#include "Snow/Scene/Scene.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/Scene/Components.h"
#include "Snow/Scene/Entity.h"
#include "Snow/Asset/AssetManager.h"

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

        {
            auto view = m_Registry.view<CircleCollider2DComponent>();
            for (auto e : view) {
                Entity entity = { e, this };

                auto& transform = entity.GetComponent<TransformComponent>();
                auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

                if (entity.HasComponent<CircleCollider2DComponent>()) {
                    SNOW_CORE_TRACE(entity.HasComponent<RigidBody2DComponent>());
                    auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

                    b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);

                    b2CircleShape shape = b2CircleShape();
                    shape.m_radius = transform.Scale.x * cc2d.Radius;

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &shape;
                    fixtureDef.density = cc2d.Density;
                    fixtureDef.friction = cc2d.Friction;
                    fixtureDef.restitution = cc2d.Restitution;
                    fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
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
                //SNOW_CORE_TRACE("Entity: {0}", e.GetComponent<TagComponent>().Tag.c_str());
                //SNOW_CORE_TRACE("   Supposed Y: {0}", transform.Transform[3].y);
                //SNOW_CORE_TRACE("   Box2D Y: {0}", position.y);
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
        
        {
            m_LightEnvironment = LightEnvironment();

            {
                auto lights = m_Registry.view<DirectionalLightComponent, TransformComponent>();
                uint32_t directionLightIndex = 0;
                for (auto entity : lights) {
                    auto [lightComp, transform] = lights.get<DirectionalLightComponent, TransformComponent>(entity);
                    //glm::vec3 direction = -glm::normalize(glm::mat3(transform.GetTransform()) * glm::vec3(1.0f));
                    m_LightEnvironment.DirectionalLights[directionLightIndex++] = {
                        transform.Rotation, lightComp.Radiance, lightComp.Intensity, 
                        lightComp.CastShadows, lightComp.SoftShadows
                    };
                }

                auto pointLights = m_Registry.view<TransformComponent, PointLightComponent>();
                uint32_t pointLightIndex = 0;
                for (auto entity : pointLights) {
                    auto [transform, lightComp] = pointLights.get<TransformComponent, PointLightComponent>(entity);

                    m_LightEnvironment.PointLights[pointLightIndex++] = {
                        transform.Translation,
                        lightComp.Intensity,
                        lightComp.Radiance,
                        lightComp.LightSize,
                        lightComp.MinRadius,
                        lightComp.Radius,
                        lightComp.Falloff
                    };
                }
            }
        }
        
        {
            auto lights = m_Registry.view<TransformComponent, SkyLightComponent>();
            if (lights.empty())
                m_Environment = Ref<Environment>::Create(Renderer::GetBlackCubeTexture(), Renderer::GetBlackCubeTexture());

            for (auto entity : lights) {
                auto [transform, skyLight] = lights.get<TransformComponent, SkyLightComponent>(entity);

                if (!AssetManager::IsAssetHandleValid(skyLight.SceneEnvironment) && skyLight.DynamicSky) {
                    Ref<TextureCube> preethamSky = Renderer::CreatePreethamSky(skyLight.TurbidityAzimuthInclination.x, skyLight.TurbidityAzimuthInclination.y, skyLight.TurbidityAzimuthInclination.z);

                    skyLight.SceneEnvironment = AssetManager::CreateMemoryOnlyAsset<Environment>(preethamSky, preethamSky);
                }
                m_Environment = AssetManager::GetAsset<Environment>(skyLight.SceneEnvironment);
                m_EnvironmentIntensity = skyLight.Intensity;
                m_SkyboxLod = skyLight.Lod;
                if (m_Environment) {
                    //    SetSkybox(m_Environment->RadianceMap);
                }
            }
        }
        
        renderer->SetScene(this);
        renderer->BeginScene({ camera, cameraViewMatrix, 0.1f, 1000.0f, 45.0f });
        //Render::SceneRenderer::BeginScene( { camera, cameraViewMatrix });
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent, BRDFMaterialComponent>();
            for (auto entity : group) {
                //auto [transform, mesh, material] = group.get<TransformComponent, MeshComponent, BRDFMaterialComponent>(entity);

                //if (mesh.Mesh.Raw() != nullptr) {
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
                //}
            }
        }
        renderer->EndScene();
        if (renderer->GetFinalPassImage()) {
            m_SceneRenderer2D->BeginScene(cameraViewProj, cameraViewMatrix);
            m_SceneRenderer2D->SetTargetRenderPass(renderer->GetCompositeRenderPass());
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                //auto texture = AssetManager::GetAsset<Texture2D>(sprite.Texture);

                if (AssetManager::IsAssetHandleValid(sprite.Texture)) {
                    Ref<Texture2D> texture = AssetManager::GetAsset<Texture2D>(sprite.Texture);
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), texture, sprite.Color);
                }
                else
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), sprite.Color);
            }

            m_SceneRenderer2D->EndScene();
        }
        //Render::SceneRenderer::EndScene();
          
        
    }

    void Scene::OnRenderEditor(Ref<Render::SceneRenderer> renderer, Timestep ts, Editor::EditorCamera& editorCamera) {
        //Render::SceneRenderer::BeginScene(editorCamera);
        
        {
            m_LightEnvironment = LightEnvironment();

            {
                auto lights = m_Registry.view<DirectionalLightComponent, TransformComponent>();
                uint32_t directionLightIndex = 0;
                for (auto entity : lights) {
                    auto [lightComp, transform] = lights.get<DirectionalLightComponent, TransformComponent>(entity);
                    glm::vec3 direction = -glm::normalize(glm::mat3(transform.GetTransform()) * glm::vec3(1.0f));
                    m_LightEnvironment.DirectionalLights[directionLightIndex++] = {
                        direction, lightComp.Radiance, lightComp.Intensity, lightComp.CastShadows, lightComp.SoftShadows
                    };
                }

                auto pointLights = m_Registry.view<TransformComponent, PointLightComponent>();
                uint32_t pointLightIndex = 0;
                for (auto entity : pointLights) {
                    auto [transform, lightComp] = pointLights.get<TransformComponent, PointLightComponent>(entity);

                    m_LightEnvironment.PointLights[pointLightIndex++] = {
                        transform.Translation,
                        lightComp.Intensity,
                        lightComp.Radiance,
                        lightComp.LightSize,
                        lightComp.MinRadius,
                        lightComp.Radius,
                        lightComp.Falloff,

                        lightComp.CastShadows
                    };
                }
            }
        }
        
        {
            auto lights = m_Registry.view<TransformComponent, SkyLightComponent>();
            if (lights.empty())
                m_Environment = Ref<Environment>::Create(Renderer::GetBlackCubeTexture(), Renderer::GetBlackCubeTexture());

            for (auto entity : lights) {
                auto [transform, skyLight] = lights.get<TransformComponent, SkyLightComponent>(entity);

                if (!AssetManager::IsAssetHandleValid(skyLight.SceneEnvironment) && skyLight.DynamicSky) {
                    Ref<TextureCube> preethamSky = Renderer::CreatePreethamSky(skyLight.TurbidityAzimuthInclination.x, skyLight.TurbidityAzimuthInclination.y, skyLight.TurbidityAzimuthInclination.z);

                    skyLight.SceneEnvironment = AssetManager::CreateMemoryOnlyAsset<Environment>(preethamSky, preethamSky);
                }
                m_Environment = AssetManager::GetAsset<Environment>(skyLight.SceneEnvironment);
                m_EnvironmentIntensity = skyLight.Intensity;
                m_SkyboxLod = skyLight.Lod;
                if (m_Environment) {
                    //    SetSkybox(m_Environment->RadianceMap);
                }
            }
        }

        renderer->SetScene(this);
        renderer->BeginScene({ editorCamera, editorCamera.GetViewMatrix(), 0.1f, 1000.0f, 45.0f });
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent>();
            for (auto entity : group) {
                auto [transform, meshComp] = group.get<TransformComponent, MeshComponent>(entity);

                if (AssetManager::IsAssetHandleValid(meshComp.Mesh)) {
                    auto mesh = AssetManager::GetAsset<Mesh>(meshComp.Mesh);
                    if (!mesh->IsFlagSet(AssetFlag::Missing)) {
                        Entity e = Entity(entity, this);

                        glm::mat4 transform = e.GetComponent<TransformComponent>().GetTransform();

                        renderer->SubmitMesh(mesh, meshComp.MaterialTable, transform);
                    }
                }
            }
        }
        renderer->EndScene();

        if (renderer->GetFinalPassImage()) {
            m_SceneRenderer2D->BeginScene(editorCamera.GetViewProjectionMatrix(), editorCamera.GetViewMatrix());
            m_SceneRenderer2D->SetTargetRenderPass(renderer->GetCompositeRenderPass());
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                if (AssetManager::IsAssetHandleValid(sprite.Texture)) {
                    Ref<Texture2D> texture = AssetManager::GetAsset<Texture2D>(sprite.Texture);
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), texture, sprite.Color);
                }
                else
                    m_SceneRenderer2D->DrawQuad(transform.GetTransform(), sprite.Color);
            }

            m_SceneRenderer2D->EndScene();
        }
        
        
    }

    void Scene::OnViewportResize(Ref<Render::SceneRenderer> renderer, uint32_t width, uint32_t height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComp = view.get<CameraComponent>(entity);
            if (!cameraComp.FixedAspectRatio)
                cameraComp.Camera.SetViewportSize(width, height);
        }

        renderer->OnViewportResize(width, height);
    }

    template<typename... Component>
    static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap) {
        ([&](){
            auto view = srcRegistry.view<Component>();
            for (auto srcEntity : view) {
                entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);

                auto& srcComp = srcRegistry.get<Component>(srcEntity);
                auto& dstComp = dstRegistry.emplace_or_replace<Component>(destEntity, srcComp);
            }
        }(), ...);
    }

    template<typename... Component>
    static void CopyComponent(ComponentGroup<Component...>, entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap) {
        CopyComponent<Component...>(dstRegistry, srcRegistry, enttMap);
    }

    template<typename... Component>
    static void CopyComponentIfExists(Entity dst, Entity src) {
        ([&]() {
            if (src.HasComponent<Component>())
                dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
        }(), ...);
    }

    template<typename... Component>
    static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dstEntity, Entity srcEntity) {
        CopyComponentIfExists<Component...>(dstEntity, srcEntity);
    }

    static void CopyAllComponents(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap) {
        CopyComponent(AllComponents{}, dstRegistry, srcRegistry, enttMap);
    }

    static void CopyAllExistingComponents(Entity dstEntity, Entity srcEntity) {
        CopyComponentIfExists(AllComponents{}, dstEntity, srcEntity);
    }
    

    Ref<Scene> Scene::CopyScene(Ref<Scene> scene) {
        Ref<Scene> newScene = Ref<Scene>::Create("Testing Name");


        auto& srcRegistry = scene->m_Registry;
        auto& dstRegistry = newScene->m_Registry;

        std::unordered_map<UUID, entt::entity> enttMap;
        auto idComponents = srcRegistry.view<IDComponent>();

        for (auto entity : idComponents) {
            auto uuid = srcRegistry.get<IDComponent>(entity).ID;
            const auto& name = srcRegistry.get<TagComponent>(entity).Tag;
            Entity e = newScene->CreateEntityWithID(uuid, name, true);
            enttMap[uuid] = e.m_EntityHandle;
        }

        CopyAllComponents(dstRegistry, srcRegistry, enttMap);

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

        CopyAllExistingComponents(newEntity, entity);

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
                return Entity{ entity, this };
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