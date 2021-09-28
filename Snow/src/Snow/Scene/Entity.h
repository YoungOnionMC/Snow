#pragma once

#include <entt.hpp>

#include "Snow/Scene/Scene.h"
#include "Snow/Scene/Components.h"


namespace Snow {
    namespace Script {
        class ScriptEngine;

    }

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            T& comp = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            return comp;
        }

        template<typename T, typename... Args>
        T& AddOrReplaceComponent(Args&&... args) {
            T& comp = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
            return comp;
        }

        template<typename T>
        T& GetComponent() {
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent() {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent() const {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent() {
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        TransformComponent Transform() { return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle); }
        const glm::mat4& Transform() const { return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle).GetTransform(); }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator==(const Entity& other) const {
            return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

        UUID GetUUID() { return GetComponent<IDComponent>().ID; }
        UUID GetSceneUUID() { return m_Scene->GetUUID(); }
    private:
        entt::entity m_EntityHandle{entt::null};
        Scene* m_Scene = nullptr;

        friend class Scene;
        friend class Script::ScriptEngine;
    };
}