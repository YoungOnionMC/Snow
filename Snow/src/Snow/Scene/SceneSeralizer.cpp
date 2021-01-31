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
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

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

		if (entity.HasComponent<ScriptComponent>()) {
			auto& sc = entity.GetComponent<ScriptComponent>();
			sc.Serialize(out, entity);
			
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeText(const std::string& filepath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_SceneID;
		out << YAML::Key << "Entities" << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.Raw() };
			if (!entity || !entity.HasComponent<IDComponent>())
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

		uint64_t uuid = data["Scene"].as<uint64_t>();
		SNOW_CORE_TRACE("Deserializing Scene with id = {0}", uuid);
		m_Scene->m_SceneID = uuid;

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				auto tagComp = TagComponent();
				if (!tagComp.Deserialize(entity)) {
					SNOW_CORE_ERROR("Entity does not have a tag component");
				}

				SNOW_CORE_INFO("Deserialized entity with ID = {0}, name = {1}", uuid, tagComp.Tag);
				Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid, tagComp.Tag);

				TransformComponent transformComp;
				if (TransformComponent::Deserialize(entity, transformComp)) {
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComp.Translation;
					tc.Rotation = transformComp.Rotation;
					tc.Scale = transformComp.Scale;
				}
				
				SpriteRendererComponent spriteRendererComp;
				if (SpriteRendererComponent::Deserialize(entity, spriteRendererComp)) {
					deserializedEntity.AddComponent<SpriteRendererComponent>(spriteRendererComp);
				}

				RigidBody2DComponent rb2dComp;
				rb2dComp = RigidBody2DComponent(RigidBody2D(m_Scene->GetPhysicsWorld(), transformComp.GetTransform()));
				if (RigidBody2DComponent::Deserialize(entity, rb2dComp)) {
					deserializedEntity.AddComponent<RigidBody2DComponent>(rb2dComp);
					
				}

				ScriptComponent sComp;
				if (ScriptComponent::Deserialize(entity, sComp, deserializedEntity)) {
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>(sComp.ModuleName);
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath) {
		return false;
	}
}