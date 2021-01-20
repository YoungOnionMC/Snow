#include <spch.h>

#include "Snow/Scene/Components.h"

namespace YAML {
    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };  

    Emitter& operator<<(Emitter& out, const ::glm::vec2& vec) {
        out << Flow;
        out << BeginSeq << vec.x << vec.y << EndSeq;
        return out;
    }

    Emitter& operator<<(Emitter& out, const ::glm::vec3& vec) {
        out << Flow;
        out << BeginSeq << vec.x << vec.y << vec.z << EndSeq;
        return out;
    }

    Emitter& operator<<(Emitter& out, const ::glm::vec4& vec) {
        out << Flow;
        out << BeginSeq << vec.x << vec.y << vec.z << vec.w << EndSeq;
        return out;
    }
}

namespace Snow {
    void TagComponent::Serialize(YAML::Emitter& out) {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Tag" << YAML::Value << Tag;
        out << YAML::EndMap;
    }

    bool TagComponent::Deserialize(YAML::Node node) {
        auto tc = node["TagComponent"];
        if (tc) {
            Tag = tc["Tag"].as<std::string>();
            return true;
        }
        return false;
    }

    void TransformComponent::Serialize(YAML::Emitter& out) {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Translation" << YAML::Value << Translation;
        out << YAML::Key << "Rotation" << YAML::Value << Rotation;
        out << YAML::Key << "Scale" << YAML::Value << Scale;

        out << YAML::EndMap; // TransformComponent
    }

    bool TransformComponent::Deserialize(YAML::Node node, TransformComponent& outTC) {
        auto tc = node["TransformComponent"];
        if (tc) {
            outTC.Translation = tc["Translation"].as<glm::vec3>();
            outTC.Rotation = tc["Rotation"].as<glm::vec3>();
            outTC.Scale = tc["Scale"].as<glm::vec3>();
            return true;
        }
        return false;
    }

    void SpriteRendererComponent::Serialize(YAML::Emitter& out) {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Color" << YAML::Value << Color;

        out << YAML::EndMap; // TransformComponent
    }

    bool SpriteRendererComponent::Deserialize(YAML::Node node, SpriteRendererComponent& outSRC) {
        auto src = node["SpriteRendererComponent"];
        if (src) {
            outSRC.Color = src["Color"].as<glm::vec4>();
            return true;
        }
        return false;
    }

    void RigidBody2DComponent::Serialize(YAML::Emitter& out) {
        out << YAML::Key << "RigidBody2DComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Type" << YAML::Value << GetType();
        out << YAML::Key << "Position" << YAML::Value << GetPosition();
        out << YAML::Key << "Size" << YAML::Value << GetSize();
        out << YAML::Key << "Density" << YAML::Value << GetDensity();
        out << YAML::Key << "Friction" << YAML::Value << GetFriction();

        out << YAML::EndMap;
        
    }

    bool RigidBody2DComponent::Deserialize(YAML::Node node, RigidBody2DComponent& outRB2D) {
        auto src = node["RigidBody2DComponent"];
        if (src) {
            outRB2D.SetType((RigidBodyType)src["Type"].as<uint32_t>());
            outRB2D.SetPosition(src["Position"].as<glm::vec2>());
            outRB2D.SetSize(src["Size"].as<glm::vec2>());
            outRB2D.SetDensity(src["Density"].as<float>());
            outRB2D.SetFriction(src["Friction"].as<float>());

            return true;
        }
        return false;
    }
}