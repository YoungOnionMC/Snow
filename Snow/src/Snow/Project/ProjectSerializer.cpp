#include <spch.h>

#include "Snow/Project/ProjectSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Snow {
	ProjectSerializer::ProjectSerializer(Ref<Project> project) {
		m_Project = project;
	}

	void ProjectSerializer::Serialize(const std::filesystem::path& filePath) {
		const auto& config = m_Project->GetConfig();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name" << YAML::Value << config.Name;
			out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
			out << YAML::Key << "AssetRegistry" << YAML::Value << config.AssetRegistryPath.string();
			out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
			out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
			out << YAML::Key << "ReloadAssemblyOnPlay" << YAML::Value << config.ReloadAssemblyOnPlay;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& filePath) {
		std::ifstream stream(filePath);
		SNOW_CORE_ASSERT(stream);

		std::stringstream sstream;
		sstream << stream.rdbuf();

		YAML::Node data = YAML::Load(sstream.str());
		if (!data["Project"])
			return false;

		YAML::Node projectNode = data["Project"];
		if (!projectNode["Name"])
			return false;

		auto& config = m_Project->m_Config;
		config.Name = projectNode["Name"].as<std::string>();

		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.AssetRegistryPath = projectNode["AssetRegistry"].as<std::string>();

		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();

		config.StartScene = projectNode["StartScene"] ? projectNode["StartScene"].as<std::string>() : "";

		config.ReloadAssemblyOnPlay = projectNode["ReloadAssemblyOnPlay"] ? projectNode["ReloadAssemblyOnPlay"].as<bool>() : true;

		std::filesystem::path path = filePath;
		config.ProjectFileName = path.filename().string();
		config.ProjectDirectory = path.parent_path().string();

		m_Project->OnDeserialized();
		return true;
	}

}