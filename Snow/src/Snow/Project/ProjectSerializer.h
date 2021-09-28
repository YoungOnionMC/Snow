#pragma once


#include "Snow/Project/Project.h"

namespace Snow {

	class ProjectSerializer {
	public:
		ProjectSerializer(Ref<Project> project);

		void Serialize(const std::string& filePath);
		bool Deserialize(const std::string& filePath);

	private:
		Ref<Project> m_Project;
	};
}