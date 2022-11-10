#include <spch.h>
#include "Snow/Project/Project.h"

#include "Snow/Asset/AssetManager.h"

namespace Snow {
	Project::Project() {
		
	}

	Project::~Project() {}

	void Project::SetActive(Ref<Project> project) {
		if (s_ActiveProject)
			AssetManager::Shutdown();

		s_ActiveProject = project;
		if (s_ActiveProject) {
			AssetManager::Init();
		}
	}

	void Project::OnDeserialized() {

	}
}