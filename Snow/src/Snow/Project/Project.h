#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Utils/FileSystem.h"

#include <filesystem>

namespace Snow {
	struct ProjectConfig {
		std::string Name;

		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistryPath;

		std::filesystem::path ScriptModulePath;

		std::filesystem::path StartScene;

		bool ReloadAssemblyOnPlay;

		std::filesystem::path ProjectFileName;
		std::filesystem::path ProjectDirectory;
	};

	class Project : public RefCounted {
	public:
		Project();
		~Project();

		static Ref<Project> GetActive() { return s_ActiveProject; }
		static void SetActive(Ref<Project> project);

		const ProjectConfig& GetConfig() const { 
			SNOW_CORE_ASSERT(s_ActiveProject);
			return m_Config; 
		}

		void SetConfig(const ProjectConfig& cfg) {
			this->m_Config = cfg;
		}

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
			return s_ActiveProject->GetConfig().ProjectDirectory / s_ActiveProject->GetConfig().AssetDirectory;
		}

		static std::filesystem::path GetAssetRegistryPath() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().ProjectDirectory / s_ActiveProject->GetConfig().AssetRegistryPath;
		}

		static std::filesystem::path GetScriptModulePath() {
			SNOW_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetConfig().ProjectDirectory / s_ActiveProject->GetConfig().ScriptModulePath;
		}

	private:
		void OnDeserialized();

		ProjectConfig m_Config;

		inline static Ref<Project> s_ActiveProject;

		friend class ProjectSerializer;
	};
}