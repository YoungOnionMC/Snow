#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/EditorCamera.h"
#include "Snow/Render/API/Texture.h"

#include "Snow/Core/Timestep.h"

#include <entt.hpp>

#include <glm/glm.hpp>

#include <box2d/b2_world.h>

namespace Snow {

    class Entity;

    struct Light {
        glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
        float padding = 0.0f;
        glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };

        float Multiplier = 1.0f;
    };

    class Scene : public RefCounted {
    public:

        Scene(const std::string& name = "Unnamed Scene");
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(Timestep ts);
        void OnUpdateEditor(Timestep ts, Render::EditorCamera& editorCamera);
        void OnViewportResize(uint32_t width, uint32_t height);

        Light& GetLight() { return m_Light; }
        const Light& GetLight() const { return m_Light; }

        b2World* GetPhysicsWorld() const { return m_PhysicsWorld; }
        
        Entity GetMainCamera();
    private:


        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        Light m_Light;
        float m_LightMultiplier = 0.3f;

        Ref<Render::API::TextureCube> m_EnvMap;
        Ref<Render::API::Texture2D> m_BRDFLUT;

        std::string m_Name;

        b2Vec2 m_Gravity = b2Vec2(0.0f, -1.0f);
        b2World* m_PhysicsWorld;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneRenderer;
        friend class SceneHierarchyPanel;
    };

}