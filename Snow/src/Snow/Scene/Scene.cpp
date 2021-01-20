#include <spch.h>
#include "Snow/Scene/Scene.h"

#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/Scene/Components.h"
#include "Snow/Scene/Entity.h"

#include <glm/glm.hpp>

#include "Snow/Render/SceneRenderer.h"

namespace Snow {
    Scene::Scene(const std::string& name) :
        m_Name(name) {
        m_Light.Direction = glm::vec3(0.1, 0.0, 1.0);
        m_Light.Radiance = glm::vec3(1.0, 1.0, 1.0);
        m_PhysicsWorld = new b2World(m_Gravity);
        m_EnvMap = Render::API::TextureCube::Create("assets/textures/SunnyTextureCube.png");
        m_BRDFLUT = Render::API::Texture2D::Create("assets/textures/BRDFLUT.png");
    }

    Scene::~Scene() {}

    Entity Scene::CreateEntity(const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdateRuntime(Timestep ts) {

        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
                if (!nsc.Instance) {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = Entity{ entity, this };
                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate(ts);

            });
        }

        Entity cameraEntity = GetMainCamera();
        if (!cameraEntity)
            return;


        glm::mat4 cameraViewMatrix = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        SceneCamera camera = cameraEntity.GetComponent<CameraComponent>().Camera;
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
        {
            auto group = m_Registry.view<TransformComponent, RigidBody2DComponent>();
            for (auto entity : group) {
                auto [transform, rigidBody] = group.get<TransformComponent, RigidBody2DComponent>(entity);

                if (rigidBody.GetPosition() != glm::vec2(transform.Translation) && (rigidBody.GetType() != (uint32_t)RigidBodyType::Static))
                    transform.Translation = { rigidBody.GetPosition(), transform.Translation.z };
            }
        }



        Render::SceneRenderer::EndScene();
            
        m_PhysicsWorld->Step(ts, 6, 2);
    }

    void Scene::OnUpdateEditor(Timestep ts, Render::EditorCamera& editorCamera) {
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

    Entity Scene::GetMainCamera() {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto camera = view.get<CameraComponent>(entity);
            if (camera.Primary) {
                return { entity, this };
            }
        }
        return {};
    }
}