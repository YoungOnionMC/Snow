#pragma once

#include "Snow/Render/Shader/Shader.h"

#include <unordered_map>

namespace Snow {
	namespace Render {
		class ShaderLibrary : public RefCounted {
		public:
			ShaderLibrary();
			~ShaderLibrary();

			void Add(const Ref<Shader>& shader);
			void Load(ShaderType type, const std::string& path);
			void Load(ShaderType type, const std::string& name, const std::string& path);

			const Ref<Shader>& Get(const std::string& name) const;
		private:
			std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		};
	}
}
