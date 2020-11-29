#pragma once

#include "Snow/Core/Ref.h"

#include <entt.hpp>

namespace Snow {

    class Entity;

    class Scene : public RefCounted {
    public:
        Scene(const std::string& name = "Unnamed Scene");
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate();
        void OnViewportResize(uint32_t width, uint32_t height);
    private:

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;

        std::string m_Name;


    };

}