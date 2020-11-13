#include <spch.h>
#include "Snow/Scene/Scene.h"

#include "Snow/Render/Renderer2D.h"

#include "Snow/Scene/Components.h"
#include "Snow/Scene/Entity.h"

#include "Snow/Math/Math.h"

namespace Snow {
    Scene::Scene() {}

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
        Math::Matrix4x4f cameraTransform = Math::Matrix4x4f(1.0f);
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

                Render::Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }

            Render::Renderer2D::EndScene();


        }
    }
}