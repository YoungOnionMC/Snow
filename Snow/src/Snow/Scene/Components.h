#pragma once

#include <string>

#include "Snow/Scene/SceneCamera.h"
//#include "Snow/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Snow/Render/Texture.h"

#include "Snow/Render/Mesh.h"
#include "Snow/Render/Material.h"
#include "Snow/Render/MaterialAsset.h"

#include "Snow/Math/Mat4.h"
#include "Snow/Physics/2D/RigidBody2D.h"

#include "Snow/Script/ScriptModuleField.h"

#include "Snow/Core/UUID.h"

#include <string>

#include <yaml-cpp/yaml.h>

#include <box2d/box2d.h>


namespace Snow {
    struct IDComponent {
        UUID ID = 0;
    };

    struct TagComponent {
        std::string Tag;

        TagComponent() = default;;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) :
            Tag(tag) {}

        operator std::string& () { return Tag; }
        operator const std::string& () const { return Tag; }

        void Serialize(YAML::Emitter& out);
        bool Deserialize(YAML::Node node);
    };

    struct KinComponent {
        UUID Parent;
        std::vector<UUID> Children;

        void Serialize(YAML::Emitter& out);
        bool Deserialize(YAML::Node node);
    };

    struct TransformComponent {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        glm::mat4 Transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) :
            Translation(translation) {}

        void SetTranslation(const glm::vec3& translation) {
            Translation = translation;
        }

        void SetRotation(const glm::vec3& rotation) {
            Rotation = rotation;
        }

        void SetScale(const glm::vec3& scale) {
            Scale = scale;
        }

        glm::mat4 GetTransform() {
            Transform = glm::translate(glm::mat4(1.0f), Translation) * glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
            return Transform;
        }

        void SetTransform(const glm::mat4& transform) {
            Transform = transform;
            Math::DecomposeTransform(Transform, Translation, Rotation, Scale);
        }

        operator glm::mat4& () { return GetTransform(); }
        //operator const glm::mat4& () const { return GetTransform(); }

        void Serialize(YAML::Emitter& out);
        static bool Deserialize(YAML::Node node, TransformComponent& outTC);
    };

    struct SpriteRendererComponent {
        glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

        AssetHandle Texture;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) :
            Color(color) {}

        void Serialize(YAML::Emitter& out);
        static bool Deserialize(YAML::Node node, SpriteRendererComponent& outSRC);
    };

    struct CameraComponent {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() {
            Camera.SetViewportSize(1280, 720);
            Camera.SetPerspective(glm::radians(45.0f), 0.1f, 1000.0f);
            //Camera.SetOrthographic(20, -1.0f, 1.0f);
        }
        CameraComponent(const CameraComponent&) = default;

    };

    struct BRDFMaterialComponent {
        //Ref<Render::MaterialInstance> MaterialInstance;

        struct Albedo {
            Ref<Render::Texture2D> AlbedoTexture;
            glm::vec3 AlbedoColor = { 1.0, 1.0, 1.0 };
            bool UseTexture = false;
        };
        Albedo AlbedoInput;

        struct Normal {
            Ref<Render::Texture2D> NormalTexture;
            bool UseTexture = false;
        };
        Normal NormalInput;

        struct Metalness {
            Ref<Render::Texture2D> MetalnessTexture;
            float Value = 0.0f;
            bool UseTexture = false;
        };
        Metalness MetalnessInput;

        struct Roughness {
            Ref<Render::Texture2D> RoughnessTexture;
            float Value = 0.2f;
            bool UseTexture = false;
        };
        Roughness RoughnessInput;

        BRDFMaterialComponent() = default;
        BRDFMaterialComponent(const BRDFMaterialComponent& other) = default;
        //BRDFMaterialComponent(const Ref<Render::MaterialInstance>& materialInstance) :
        //    MaterialInstance(materialInstance) {}
    };

    struct MeshComponent {
        AssetHandle Mesh;
        Ref<Snow::MaterialTable> MaterialTable = Ref<Snow::MaterialTable>::Create(0);

        MeshComponent() = default;
        MeshComponent(const MeshComponent& other) :
            Mesh(other.Mesh), MaterialTable(Ref<Snow::MaterialTable>::Create(other.MaterialTable)) {}

        MeshComponent(AssetHandle mesh) :
            Mesh(mesh) {}

        ~MeshComponent() {
        
        }
    };

    class Entity;
    struct ScriptComponent {
        std::string ModuleName;
        Script::ScriptModuleFieldMap ModuleFieldMap;

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent& other) = default;
        ScriptComponent(const std::string& moduleName) :
            ModuleName(moduleName) {}

        void Serialize(YAML::Emitter& out, Entity entity);
        static bool Deserialize(YAML::Node node, ScriptComponent& outRB2D, Entity entity);
    };

    struct RigidBody2DComponent {
        enum class BodyType {Static = 0, Dynamic, Kinematic};
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        void* RuntimeBody = nullptr;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(const RigidBody2DComponent&) = default;

        void Serialize(YAML::Emitter& out);
        static bool Deserialize(YAML::Node node, RigidBody2DComponent& outRB2D);
    };

    struct BoxCollider2DComponent {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 0.5f, 0.5f };

        float Density = 1.0f;
        float Friction = 0.4f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.6f;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

        void Serialize(YAML::Emitter& out) {}
        static bool Deserialize(YAML::Node node, BoxCollider2DComponent& outbc2d) {}
    };

    struct CircleCollider2DComponent {
        float Radius = 1.0f;
        glm::vec2 Offset = { 0.0f, 0.0f };

        float Density = 1.0f;
        float Friction = 0.4f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.6f;
    };

    enum class LightType {
        None = 0, Directional = 1, Point = 2, Spot = 3
    };

    struct DirectionalLightComponent {
        glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
        float Intensity = 1.0f;

        bool CastShadows = true;
        bool SoftShadows = true;

        DirectionalLightComponent() = default;
        DirectionalLightComponent(const DirectionalLightComponent&) = default;
    };

    struct PointLightComponent {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        float Intensity = 1.0f;
        glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
        float LightSize = 0.5;
        float MinRadius = 1.0f;
        float Radius = 10.0f;
        float Falloff = 1.0f;

        bool CastShadows = true;

        PointLightComponent() = default;
        PointLightComponent(const PointLightComponent&) = default;
    };

    struct SkyLightComponent {
        AssetHandle SceneEnvironment;
        float Intensity = 1.0f;
        float Lod = 0.0f;

        bool DynamicSky = false;
        glm::vec3 TurbidityAzimuthInclination = { 2.0, 0.0, 0.0 };
    };

    template<typename... Component>
    struct ComponentGroup {};
    using AllComponents = ComponentGroup <
        TransformComponent,

        SpriteRendererComponent,
        CameraComponent,

        MeshComponent,

        //DirectionalLightComponent,
        //PointLightComponent,
        SkyLightComponent,

        RigidBody2DComponent,
        BoxCollider2DComponent,
        CircleCollider2DComponent,

        ScriptComponent
    >;
}