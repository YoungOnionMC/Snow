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
			void Load(std::initializer_list<ShaderModule> shaderModules);
			void Load(std::initializer_list<ShaderModule> shaderModules, const std::string& name);

			const Ref<Shader>& Get(const std::string& name) const;
			std::unordered_map<std::string, Ref<Shader>>& Get() { return m_Shaders; }
		private:
			std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		};
	}
}
