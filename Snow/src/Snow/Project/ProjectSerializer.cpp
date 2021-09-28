#include <spch.h>

#include "Snow/Project/ProjectSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Snow {
	ProjectSerializer::ProjectSerializer(Ref<Project> project) {
		m_Project = project;
	}

	void ProjectSerializer::Serialize(const std::string& filePath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << m_Project->m_Config.Name;
			out << YAML::Key << "AssetDirectory" << YAML::Value << m_Project->m_Config.AssetDirectory;
			out << YAML::Key << "AssetRegistry" << YAML::Value << m_Project->m_Config.AssetRegistryPath;
			out << YAML::Key << "ScriptModulePath" << YAML::Value << m_Project->m_Config.ScriptModulePath;
			out << YAML::Key << "StartScene" << YAML::Value << m_Project->m_Config.StartScene;
			out << YAML::Key << "ReloadAssemblyOnPlay" << YAML::Value << m_Project->m_Config.ReloadAssemblyOnPlay;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool ProjectSerializer::Deserialize(const std::string& filePath) {
		std::ifstream stream(filePath);
		SNOW_CORE_ASSERT(stream);

		std::stringstream sstream;
		sstream << stream.rdbuf();

		YAML::Node data = YAML::Load(sstream.str());
		if (!data["Project"])
			return false;

		YAML::Node baseNode = data["Project"];
		if (!baseNode["Name"])
			return false;

		auto& config = m_Project->m_Config;
		config.Name = baseNode["Name"].as<std::string>();

		config.AssetDirectory = baseNode["AssetDirectory"].as<std::string>();
		config.AssetRegistryPath = baseNode["AssetRegistry"].as<std::string>();

		config.ScriptModulePath = baseNode["ScriptModulePath"].as<std::string>();

		config.StartScene = baseNode["StartScene"] ? baseNode["StartScene"].as<std::string>() : "";

		config.ReloadAssemblyOnPlay = baseNode["ReloadAssemblyOnPlay"] ? baseNode["ReloadAssemblyOnPlay"].as<bool>() : true;

		std::filesystem::path path = filePath;
		config.ProjectFileName = path.filename().string();
		config.ProjectDirectory = path.parent_path().string();

		m_Project->OnDeserialized();
		return true;
	}

}