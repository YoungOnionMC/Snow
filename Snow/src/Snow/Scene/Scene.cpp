#include <spch.h>
#include "Snow/Scene/Scene.h"

#include "Snow/Render/Renderer2D.h"

#include "Snow/Scene/Components.h"
#include "Snow/Scene/Entity.h"

#include <glm/glm.hpp>

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
        Render::Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform = glm::mat4(1.0f);
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for(auto entity : view) {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                if(camera.Primary) {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }

        if(mainCamera) {
            Render::Renderer2D::BeginScene(*mainCamera, cameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for(auto entity : group) {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                if (!sprite.Texture)
                    Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
                else
                    Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color);
            }

            Render::Renderer2D::EndScene();
            

        }
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
}