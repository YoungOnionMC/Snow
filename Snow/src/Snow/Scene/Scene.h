#pragma once

#include "Snow/Core/Ref.h"

#include <entt.hpp>

namespace Snow {

    class Entity;

    class Scene : public RefCounted {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate();
        void OnViewportResize(uint32_t width, uint32_t height);


    private:

        entt::registry m_Registry;

        friend class Entity;
        friend class SceneHierarchyPanel;

        


    };

}