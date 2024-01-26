#pragma once


#include "Snow/Project/Project.h"

#include <filesystem>

namespace Snow {

	class ProjectSerializer {
	public:
		ProjectSerializer(Ref<Project> project);

		void Serialize(const std::filesystem::path& filePath);
		bool Deserialize(const std::filesystem::path& filePath);

	private:
		Ref<Project> m_Project;
	};
}