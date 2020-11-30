#pragma once

#include "Snow/Core/Ref.h"

#include <entt.hpp>

#include <glm/glm.hpp>

namespace Snow {

    class Entity;

    struct Light {
        glm::vec3 Direction = {0.0f, 0.0f, 0.0f};
        float pad = 0.0f;
        glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
        float Multiplier = 1.0f;
    };

    class Scene : public RefCounted {
    public:

        Scene(const std::string& name = "Unnamed Scene");
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate();
        void OnViewportResize(uint32_t width, uint32_t height);

        Light& GetLight() { return m_Light; }
        const Light& GetLight() const { return m_Light; }
    private:

        Entity GetMainCamera();

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneRenderer;
        friend class SceneHierarchyPanel;

        std::string m_Name;

        Light m_Light;
    };

}