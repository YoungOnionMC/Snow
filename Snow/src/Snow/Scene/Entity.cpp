#include <spch.h>
#include "Snow/Scene/Entity.h"

#include "Snow/Scene/Components.h"

namespace Snow {
    Entity::Entity(entt::entity handle, Scene* scene) :
        m_EntityHandle(handle), m_Scene(scene) {}

    UUID Entity::GetUUID() {
        return GetComponent<IDComponent>().ID;
    }

    glm::mat4& Entity::GetTransform() { 
        SNOW_CORE_ASSERT(HasComponent<TransformComponent>(), "Entity does not have TransformComponent");
        return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle); 
    }

    const glm::mat4& Entity::GetTransform() const { 
        SNOW_CORE_ASSERT(HasComponent<TransformComponent>(), "Entity does not have TransformComponent"); 
        return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle); 
    }
    
}