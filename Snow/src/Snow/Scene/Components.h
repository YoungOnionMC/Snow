#pragma once

#include <string>

#include "Snow/Scene/SceneCamera.h"
//#include "Snow/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    struct TransformComponent {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        glm::mat4 Transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) :
            Translation(translation) {
            UpdateTransform();
        }

        void SetTranslation(const glm::vec3& translation) {
            Translation = translation;
            UpdateTransform();
        }

        void SetRotation(const glm::vec3& rotation) {
            Rotation = rotation;
            UpdateTransform();
        }

        void SetScale(const glm::vec3& scale) {
            Scale = scale;
            UpdateTransform();
        }

        void UpdateTransform() {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
                * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
                * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

            Transform = glm::translate(glm::mat4(1.0f), Translation) *
                rotation * glm::scale(glm::mat4(1.0f), Scale);
        }

        glm::mat4 GetTransform() {
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

        Ref<Render::Texture2D> Texture;

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
        Ref<Render::Mesh> Mesh;
        Ref<Snow::MaterialTable> MaterialTable = Ref<Snow::MaterialTable>::Create(0);

        MeshComponent() = default;
        MeshComponent(Ref<Render::Mesh> mesh) :
            Mesh(mesh) {}
        MeshComponent(const std::string& filePath) {
            Mesh = Ref<Render::Mesh>::Create(Ref<Render::MeshAsset>::Create(filePath));
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
}