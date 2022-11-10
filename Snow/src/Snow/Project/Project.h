#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Utils/FileSystem.h"

namespace Snow {
	struct ProjectConfig {
		std::string Name;

		std::string AssetDirectory;
		std::string AssetRegistryPath;

		std::string ScriptModulePath;

		std::string StartScene;

		bool ReloadAssemblyOnPlay;

		std::string ProjectFileName;
		std::string ProjectDirectory;
	};

	class Project : public RefCounted {
	public:
		Project();
		~Project();

		static Ref<Project> GetActive() { return s_ActiveProject; }
		static void SetActive(Ref<Project> project);

		const ProjectConfig& GetConfig() const { return m_Config; }

		static const std::string& GetProjectName() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().Name;
		}

		static std::filesystem::path GetProjectDirectory() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / s_ActiveProject->GetConfig().AssetDirectory;
		}

		static std::filesystem::path GetAssetRegistryPath() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / s_ActiveProject->GetConfig().AssetRegistryPath;
		}

		static std::filesystem::path GetScriptModulePath() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / s_ActiveProject->GetConfig().ScriptModulePath;
		}

	private:
		void OnDeserialized();

		ProjectConfig m_Config;

		inline static Ref<Project> s_ActiveProject;

		friend class ProjectSerializer;
	};
}