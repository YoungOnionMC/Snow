#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/EditorCamera.h"

#include <entt.hpp>

#include <glm/glm.hpp>

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

        void OnUpdateRuntime();
        void OnUpdateEditor(Render::EditorCamera& editorCamera);
        void OnViewportResize(uint32_t width, uint32_t height);

        Light& GetLight() { return m_Light; }
        const Light& GetLight() const { return m_Light; }
        
        Entity GetMainCamera();
    private:


        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        Light m_Light;
        float m_LightMultiplier = 0.3f;

        std::string m_Name;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneRenderer;
        friend class SceneHierarchyPanel;
    };

}