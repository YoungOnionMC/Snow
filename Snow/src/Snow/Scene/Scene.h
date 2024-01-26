#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Editor/EditorCamera.h"
#include "Snow/Render/Texture.h"
#include "Snow/Render/Renderer2D.h"
#include "Snow/Scene/SceneEnvironment.h"

#include "Snow/Core/Timestep.h"
#include "Snow/Core/UUID.h"

#include <entt.hpp>

#include <glm/glm.hpp>

//#define B2_USER_SETTINGS
#include <box2d/b2_world.h>

namespace Snow {
    namespace Render {
        class SceneRenderer;
    }

    class Entity;
    using EntityMap = std::unordered_map<UUID, Entity>;


    struct DirectionalLight {
        glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };

        float Multiplier = 1.0f;

        bool CastShadows = true;
        bool SoftShadows = false;
    };

    struct PointLight {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        float Intensity = 1.0f;
        glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
        float LightSize = 0.5;
        float MinRadius = 1.0f;
        float Radius = 10.0f;
        float Falloff = 1.0f;

        bool CastShadows = false;
    };

    struct LightEnvironment {
        DirectionalLight DirectionalLights[8];
        std::vector<PointLight> PointLights;
    };

    class Scene : public Asset {
    public:

        Scene(const std::string& name = "Unnamed Scene");
        ~Scene();

        void Init();

        template<typename T>
        auto GetAllEntitiesWith() {
            return m_Registry.view<T>();
        }

        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnUpdate(Timestep ts);
        void OnRenderRuntime(Ref<Render::SceneRenderer> renderer, Timestep ts);
        void OnRenderEditor(Ref<Render::SceneRenderer> renderer, Timestep ts, Editor::EditorCamera& editorCamera);

        void OnViewportResize(Ref<Render::SceneRenderer> renderer, uint32_t width, uint32_t height);

        Entity FindEntityByTag(const std::string& tag);
        static Ref<Scene> CopyScene(Ref<Scene> scene);

        const Ref<Environment>& GetEnvironment() const { return m_Environment; }

        DirectionalLight& GetLight() { return m_Light; }
        const DirectionalLight& GetLight() const { return m_Light; }

        b2World* GetPhysicsWorld() const { return m_PhysicsWorld; }

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithID(UUID uuid, const std::string& name = std::string(), bool runtimeMap = false);
        void DestroyEntity(Entity entity);

        Entity DuplicateEntity(Entity entity);
        
        Entity GetMainCamera();

        const EntityMap& GetEntityMap() const { return m_EntityIDMap; }

        static AssetType GetStaticType() { return AssetType::Scene; }
        virtual AssetType GetAssetType() const override { return GetStaticType(); }


        UUID GetUUID() const { return m_SceneID; }
        static Ref<Scene> GetScene(UUID uuid);
    private:
        UUID m_SceneID;
        entt::entity m_SceneEntity;
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        EntityMap m_EntityIDMap;

        DirectionalLight m_Light;
        float m_LightMultiplier = 0.3f;

        LightEnvironment m_LightEnvironment;

        Ref<Environment> m_Environment;
        float m_EnvironmentIntensity = 0.0f;
        int m_SkyboxLod = 0;

        Ref<Render::Texture2D> m_BRDFLUT;

        Entity* m_Physics2DBodyEntityBuffer = nullptr;

        Ref<Render::Renderer2D> m_SceneRenderer2D;

        bool m_IsPlaying;

        std::string m_Name;

        b2Vec2 m_Gravity = b2Vec2(0.0f, -1.0f);
        b2World* m_PhysicsWorld;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneRenderer;
        friend class SceneHierarchyPanel;

        friend void OnScriptComponentConstruct(entt::registry& registry, entt::entity entity);
        friend void OnScriptComponentDestroy(entt::registry& registry, entt::entity entity);
    };

}