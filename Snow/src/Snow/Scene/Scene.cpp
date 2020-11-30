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
        m_Name(name) {}

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

    void Scene::OnUpdate() {
        Entity cameraEntity = GetMainCamera();
        if (!cameraEntity)
            return;


        glm::mat4 cameraViewMatrix = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

        SceneCamera camera = cameraEntity.GetComponent<CameraComponent>().Camera;
        camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);

        Render::Renderer2D::BeginScene(camera, cameraViewMatrix);

        {
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                if (!sprite.Texture)
                    Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
                else
                    Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color);
            }
        }

        Render::Renderer2D::EndScene();

        Render::SceneRenderer::BeginScene(this, {camera, cameraViewMatrix});
        {
            auto group = m_Registry.view<TransformComponent, MeshComponent>();
            for (auto entity : group) {
                auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);

                if (mesh.Mesh.Raw() != nullptr)
                    Render::SceneRenderer::SubmitMesh(mesh.Mesh, transform.GetTransform());
            }
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