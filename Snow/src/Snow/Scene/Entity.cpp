#include <spch.h>
#include "Snow/Scene/Entity.h"

namespace Snow {
    Entity::Entity(entt::entity handle, Scene* scene) :
        m_EntityHandle(handle), m_Scene(scene) {}
}