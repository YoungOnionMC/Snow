#pragma once

#include <string>

#include "Snow/Math/Math.h"

#include "Snow/Scene/SceneCamera.h"

namespace Snow {
    struct TagComponent {
        std::string Tag;

        TagComponent() = default;;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) :
            Tag(tag) {}
    };

    struct TransformComponent {
        Math::Vector3f Translation = {0.0f ,0.0f, 0.0f};
        Math::Vector3f Rotation = {0.0f, 0.0f, 0.0f};
        Math::Vector3f Scale = {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const Math::Vector3f& translation) :
            Translation(translation) {}

        Math::Matrix4x4f GetTransform() const {
            Math::Matrix4x4f rotation = Math::rotate(Math::Matrix4x4f(1.0f), Rotation.x, {1, 0, 0}) *
                                    Math::rotate(Math::Matrix4x4f(1.0f), Rotation.y, {0, 1, 0}) *
                                    Math::rotate(Math::Matrix4x4f(1.0f), Rotation.z, {0, 0, 1});

            return Math::translate(Math::Matrix4x4f(1.0f), Translation) *
                    rotation * Math::scale(Math::Matrix4x4f(1.0f), Scale);

        }
    };

    struct SpriteRendererComponent {
        Math::Vector4f Color = {1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const Math::Vector4f& color) :
            Color(color) {}
    };

    struct CameraComponent {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

    };
}