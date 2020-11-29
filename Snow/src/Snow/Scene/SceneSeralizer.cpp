#include <spch.h>
#include "Snow/Scene/SceneSerializer.h"

#include "Snow/Scene/Entity.h"

#include <yaml-cpp/yaml.h>

namespace Snow {
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
		m_Scene(scene) {}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "2345652456367"; // should be uuid



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
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath) {
		return false;
	}
}