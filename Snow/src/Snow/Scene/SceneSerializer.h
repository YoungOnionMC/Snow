#pragma once

#include "Snow/Scene/Scene.h"

namespace Snow {
	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeText(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeText(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

		inline static std::string_view FileFilter = "Snow Scene (*.sscene)\0*.sscene\0";
		inline static std::string_view DefualtExtension = ".sscene";

	private:
		Ref<Scene> m_Scene;
	};
}
