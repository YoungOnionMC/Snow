#pragma once

#include <string>

#include "Snow/Scene/SceneCamera.h"
#include "Snow/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Snow/Render/API/Texture.h"

#include "Snow/Render/Mesh.h"

#include <string>

#include <yaml-cpp/yaml.h>

#include <box2d/box2d.h>


namespace Snow {
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
        glm::vec3 Translation = { 0.0f ,0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) :
            Translation(translation) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
                * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
                * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

            return glm::translate(glm::mat4(1.0f), Translation) *
                rotation * glm::scale(glm::mat4(1.0f), Scale);

        }

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
            //Camera.SetPerspective(45.0f, 0.1f, 1000.0f);
            Camera.SetOrthographic(20, -1.0f, 1.0f);
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

    enum class RigidBodyType {
        Static = 0,
        Kinematic,
        Dynamic
    };

    struct RigidBody2DComponent {
        b2Body* Body;
        b2BodyDef* BodyDef;
        b2World* World;
        b2PolygonShape* Shape;
        b2FixtureDef* FixtureDef;
        b2Fixture* Fixture;
        glm::vec2 Size;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(b2World* world, const glm::vec2& position, const glm::vec2& size, bool dynamic, float density, float friction) {
            World = world;
            BodyDef = new b2BodyDef();
            BodyDef->type = dynamic ? b2_dynamicBody : b2_staticBody;
            BodyDef->position.Set(position.x, position.y);
            
            Body = world->CreateBody(BodyDef);

            Shape = new b2PolygonShape();
            Size = size;
            Shape->SetAsBox(size.x / 2.0f, size.y / 2.0f);
            FixtureDef = new b2FixtureDef();
            FixtureDef->shape = Shape;
            FixtureDef->density = density;
            FixtureDef->friction = friction;
            Fixture = Body->CreateFixture(FixtureDef);
        }

        glm::vec2 GetPosition() const {
            return glm::vec2(Body->GetPosition().x, Body->GetPosition().y);
        }

        void SetPosition(const glm::vec2& position) {
            Body->SetTransform({ position.x, position.y }, 0);
        }

        glm::vec2 GetSize() const { return Size; }

        void SetSize(const glm::vec2& size) {
            Shape->SetAsBox(size.x / 2.0f, size.y / 2.0f);
        }

        float GetFriction() const {
            return Fixture->GetFriction();
        }

        void SetFriction(float friction) {
            Fixture->SetFriction(friction);
        }

        float GetDensity() const {
            return Fixture->GetDensity();
        }

        void SetDensity(float density) {
            Fixture->SetDensity(density);
        }

        uint32_t GetType() const {
            return (uint32_t)Body->GetType();
        }

        void SetType(RigidBodyType type) {
            Body->SetType((b2BodyType)type);
        }

        void SetPolygonShape(const glm::vec2* points, uint32_t numPoints) {
            Shape->Set((b2Vec2*)points, numPoints);
        }

        void Serialize(YAML::Emitter& out);
        static bool Deserialize(YAML::Node node, RigidBody2DComponent& outRB2D);
    };
}