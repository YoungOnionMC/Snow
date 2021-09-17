#include <spch.h>

#include "Snow/Scene/Components.h"

#include "Snow/Script/ScriptEngine.h"

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

    void ScriptComponent::Serialize(YAML::Emitter& out, Entity entity) {
        out << YAML::Key << "ScriptComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "ModuleName" << YAML::Value << ModuleName;

        Script::EntityInstanceData& data = Script::ScriptEngine::GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID());
        const auto& moduleFieldMap = data.ModuleFieldMap;
        if (moduleFieldMap.find(ModuleName) != moduleFieldMap.end()) {
            const auto& fields = moduleFieldMap.at(ModuleName);
            out << YAML::Key << "StoredFields" << YAML::Value;
            out << YAML::BeginSeq;
            for (const auto& [name, field] : fields) {
                out << YAML::BeginMap;
                out << YAML::Key << "Name" << YAML::Value << name;
                out << YAML::Key << "Type" << YAML::Value << (uint32_t)field.Type;
                out << YAML::Key << "Data" << YAML::Value;

                switch (field.Type) {
                case Script::FieldType::Bool:
                    out << field.GetStoredValue<bool>();
                    break;
                case Script::FieldType::Short:
                    out << field.GetStoredValue<short>();
                    break;
                case Script::FieldType::Char:
                    out << field.GetStoredValue<char>();
                    break;
                case Script::FieldType::Int:
                    out << field.GetStoredValue<int>();
                    break;
                case Script::FieldType::UnsignedInt:
                    out << field.GetStoredValue<unsigned int>();
                    break;
                case Script::FieldType::Long:
                    out << field.GetStoredValue<long>();
                    break;
                case Script::FieldType::UnsignedLong:
                    out << field.GetStoredValue<unsigned long>();
                    break;
                case Script::FieldType::Float:
                    out << field.GetStoredValue<float>();
                    break;
                case Script::FieldType::Double:
                    out << field.GetStoredValue<double>();
                    break;
                case Script::FieldType::Vec2:
                    out << field.GetStoredValue<glm::vec2>();
                    break;
                case Script::FieldType::Vec3:
                    out << field.GetStoredValue<glm::vec3>();
                    break;
                case Script::FieldType::Vec4:
                    out << field.GetStoredValue<glm::vec4>();
                    break;
                }
                out << YAML::EndMap;
            }
            out << YAML::EndSeq;
        }

        out << YAML::EndMap;
    }

    bool ScriptComponent::Deserialize(YAML::Node node, ScriptComponent& sc, Entity entity) {
        auto src = node["ScriptComponent"];
        if (src) {
            sc.ModuleName = src["ModuleName"].as<std::string>();

            if (Script::ScriptEngine::ModuleExists(sc.ModuleName)) {
                auto storedFields = src["StoredFields"];
                if (storedFields) {
                    for (auto field : storedFields) {
                        std::string name = field["Name"].as<std::string>();
                        Script::FieldType type = (Script::FieldType)field["Type"].as<uint32_t>();
                        std::string typeName = "";// Script::FieldTypeToString(type);
                        Script::EntityInstanceData& data = Script::ScriptEngine::GetEntityInstanceData(entity.GetSceneUUID(), entity.GetUUID());
                        auto& moduleFieldMap = data.ModuleFieldMap;
                        auto& publicFields = moduleFieldMap[sc.ModuleName];
                        if (publicFields.find(name) == publicFields.end()) {
                            Script::PublicField f = { name,typeName, type };
                            publicFields.emplace(name, std::move(f));
                        }
                        auto dataNode = field["Data"];
                        switch (type) {
                        case Script::FieldType::Bool:
                            publicFields.at(name).SetStoredValue(dataNode.as<bool>());
                            break;
                        case Script::FieldType::Short:
                            publicFields.at(name).SetStoredValue(dataNode.as<short>());
                            break;
                        case Script::FieldType::Char:
                            publicFields.at(name).SetStoredValue(dataNode.as<char>());
                            break;
                        case Script::FieldType::Int:
                            publicFields.at(name).SetStoredValue(dataNode.as<int>());
                            break;
                        case Script::FieldType::UnsignedInt:
                            publicFields.at(name).SetStoredValue(dataNode.as<unsigned int>());
                            break;
                        case Script::FieldType::Long:
                            publicFields.at(name).SetStoredValue(dataNode.as<long>());
                            break;
                        case Script::FieldType::UnsignedLong:
                            publicFields.at(name).SetStoredValue(dataNode.as<unsigned long>());
                            break;
                        case Script::FieldType::Float:
                            publicFields.at(name).SetStoredValue(dataNode.as<float>());
                            break;
                        case Script::FieldType::Double:
                            publicFields.at(name).SetStoredValue(dataNode.as<double>());
                            break;
                        case Script::FieldType::Vec2:
                            publicFields.at(name).SetStoredValue(dataNode.as<glm::vec2>());
                            break;
                        case Script::FieldType::Vec3:
                            publicFields.at(name).SetStoredValue(dataNode.as<glm::vec3>());
                            break;
                        case Script::FieldType::Vec4:
                            publicFields.at(name).SetStoredValue(dataNode.as<glm::vec4>());
                            break;
                        }
                    }
                }
            }
            
            return true;
        }
        return false;
    }

    void RigidBody2DComponent::Serialize(YAML::Emitter& out) {
        out << YAML::Key << "RigidBody2DComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "Type" << YAML::Value << RigidBody.GetType();
        out << YAML::Key << "Position" << YAML::Value << RigidBody.GetPosition();
        out << YAML::Key << "Rotation" << YAML::Value << RigidBody.GetRotation();
        out << YAML::Key << "Size" << YAML::Value << RigidBody.GetSize();
        out << YAML::Key << "Density" << YAML::Value << RigidBody.GetDensity();
        out << YAML::Key << "Friction" << YAML::Value << RigidBody.GetFriction();

        out << YAML::EndMap;
        
    }

    bool RigidBody2DComponent::Deserialize(YAML::Node node, RigidBody2DComponent& outRB2D) {
        auto src = node["RigidBody2DComponent"];
        if (src) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), src["Rotation"].as<float>(), { 0, 0, 1 });
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(src["Position"].as<glm::vec2>(), 0.0f)) 
                * rotation 
                * glm::scale(glm::mat4(1.0f), glm::vec3(src["Size"].as<glm::vec2>(), 1.0f));


            outRB2D.RigidBody.SetType((RigidBodyType)src["Type"].as<uint32_t>());
            

            outRB2D.RigidBody.SetTransform(transform);
            outRB2D.RigidBody.SetDensity(src["Density"].as<float>());
            outRB2D.RigidBody.SetFriction(src["Friction"].as<float>());

            return true;
        }
        return false;
    }
}