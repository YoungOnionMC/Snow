#include <spch.h>
#include "Snow/Scene/SceneSerializer.h"

#include "Snow/Scene/Entity.h"
#include "Snow/Scene/Components.h"

#include <yaml-cpp/yaml.h>

namespace Snow {
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
		m_Scene(scene) {}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "2345652456367"; // should be uuid

		if (entity.HasComponent<TagComponent>()) {
			auto& tc = entity.GetComponent<TagComponent>();
			tc.Serialize(out);
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& tc = entity.GetComponent<TransformComponent>();
			tc.Serialize(out);
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			src.Serialize(out);
		}

		if (entity.HasComponent<RigidBody2DComponent>()) {
			auto& src = entity.GetComponent<RigidBody2DComponent>();
			src.Serialize(out);
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeText(const std::string& filepath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;
		out << YAML::Key << "Entities" << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.Raw() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath) {

	}

	bool SceneSerializer::DeserializeText(const std::string& filepath) {
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SNOW_CORE_TRACE("Deserializing Scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				auto tagComp = TagComponent();
				if (!tagComp.Deserialize(entity)) {
					SNOW_CORE_ERROR("Entity does not have a tag component");
				}
				Entity deserializedEntity = m_Scene->CreateEntity(tagComp.Tag);

				TransformComponent transformComp;
				if (TransformComponent::Deserialize(entity, transformComp)) {
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComp.Translation;
					tc.Rotation = transformComp.Rotation;
					tc.Scale = transformComp.Scale;
				}
				
				SpriteRendererComponent spriteRendererComp;
				if (SpriteRendererComponent::Deserialize(entity, spriteRendererComp)) {
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComp.Color;
				}

				RigidBody2DComponent rb2dComp;
				rb2dComp = RigidBody2DComponent(m_Scene->GetPhysicsWorld(), { 0,0 }, { 0,0 }, false, 0, 0);
				if (RigidBody2DComponent::Deserialize(entity, rb2dComp)) {
					auto& src = deserializedEntity.AddComponent<RigidBody2DComponent>();
					src = rb2dComp;
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath) {
		return false;
	}
}