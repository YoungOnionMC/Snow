#include <spch.h>
#include "Snow/Render/Shader/ShaderLibrary.h"

namespace Snow {
	namespace Render {
		ShaderLibrary::ShaderLibrary() {

		}

		ShaderLibrary::~ShaderLibrary() {}

		void ShaderLibrary::Add(const Ref<Shader>& shader) {
			auto& name = shader->GetName();
			SNOW_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			m_Shaders[name] = shader;
		}

		void ShaderLibrary::Load(ShaderType type, const std::string& path) {
			auto shader = Ref<Shader>(Shader::Create(type, path));
			auto& name = shader->GetName();
			SNOW_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			m_Shaders[name] = shader;
		}

		void ShaderLibrary::Load(ShaderType type, const std::string& name, const std::string& path) {
			SNOW_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			m_Shaders[name] = Ref<Shader>(Shader::Create(type, path));
		}

		const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const {
			SNOW_ASSERT(m_Shaders.find(name) != m_Shaders.end());
			return m_Shaders.at(name);
		}
	}
}