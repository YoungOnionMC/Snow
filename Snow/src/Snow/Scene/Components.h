#pragma once

#include <string>

#include "Snow/Scene/SceneCamera.h"
#include "Snow/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Snow/Render/API/Texture.h"

#include "Snow/Render/Mesh.h"

#include "Snow/Math/Mat4.h"
#include "Snow/Physics/2D/RigidBody2D.h"

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

        Ref<Render::API::Texture2D> Texture;

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
        Ref<Render::MaterialInstance> MaterialInstance;

        struct Albedo {
            Ref<Render::API::Texture2D> AlbedoTexture;
            glm::vec3 AlbedoColor = { 1.0, 1.0, 1.0 };
            bool UseTexture = false;
        };
        Albedo AlbedoInput;

        struct Normal {
            Ref<Render::API::Texture2D> NormalTexture;
            bool UseTexture = false;
        };
        Normal NormalInput;

        struct Metalness {
            Ref<Render::API::Texture2D> MetalnessTexture;
            float Value = 0.0f;
            bool UseTexture = false;
        };
        Metalness MetalnessInput;

        struct Roughness {
            Ref<Render::API::Texture2D> RoughnessTexture;
            float Value = 0.2f;
            bool UseTexture = false;
        };
        Roughness RoughnessInput;

        BRDFMaterialComponent() = default;
        BRDFMaterialComponent(const BRDFMaterialComponent& other) = default;
        BRDFMaterialComponent(const Ref<Render::MaterialInstance>& materialInstance) :
            MaterialInstance(materialInstance) {}
    };

    struct MeshComponent {
        Ref<Render::Mesh> Mesh;

        MeshComponent() = default;
        MeshComponent(const std::string& filePath) {
            Mesh = Ref<Render::Mesh>::Create(filePath);
        }
    };

    struct ScriptComponent {
        std::string ModuleName;

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent& other) = default;
        ScriptComponent(const std::string& moduleName) :
            ModuleName(moduleName) {}

        void Serialize(YAML::Emitter& out, Entity entity);
        static bool Deserialize(YAML::Node node, ScriptComponent& outRB2D, Entity entity);
    };

    struct NativeScriptComponent {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind() {
            InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
        }
    };

    struct RigidBody2DComponent {
        RigidBody2D RigidBody;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(const RigidBody2D& rigidBody) : RigidBody(rigidBody) {}

        void Serialize(YAML::Emitter& out);
        static bool Deserialize(YAML::Node node, RigidBody2DComponent& outRB2D);
    };
}