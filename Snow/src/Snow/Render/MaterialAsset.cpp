#include <spch.h>

#include "Snow/Render/MaterialAsset.h"
#include "Snow/Render/Renderer.h"

namespace Snow {
	using namespace Render;
	MaterialAsset::MaterialAsset() {
		m_Material = Material::Create(Renderer::GetShaderLibrary()->Get("PBR"));

		SetValue("u_MaterialUniforms.AlbedoColor", glm::vec3(0.8f));
		SetValue("u_MaterialUniforms.Emission", 0.0f);
		SetValue("u_MaterialUniforms.UseNormalMap", false);
		SetValue("u_MaterialUniforms.Metalness", 0.0f);
		SetValue("u_MaterialUniforms.Roughness", 0.4f);
	}

	MaterialAsset::MaterialAsset(Ref<Material> material) {
		m_Material = Material::Copy(material);
		
	}

	MaterialAsset::~MaterialAsset() {}

	void MaterialAsset::SetTexture(const std::string& name, Ref<Render::Texture2D> texture) {
		m_Material->Set(name, texture);
	}

	void MaterialAsset::ClearTexture(const std::string& name) {
		m_Material->Set(name, Render::Renderer::GetWhiteTexture());
	}

	Ref<Render::Texture2D> MaterialAsset::GetTexture(const std::string& name) {
		return m_Material->GetTexture2D(name);
	}

	void MaterialAsset::SetValue(const std::string& name, bool value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValue(const std::string& name, int value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValue(const std::string& name, float value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValue(const std::string& name, double value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValue(const std::string& name, const glm::vec2& value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValue(const std::string& name, const glm::vec3& value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValue(const std::string& name, const glm::vec4& value) {
		m_Material->Set(name, value);
	}

	void MaterialAsset::SetValuesInMap() {
		for (auto [name, type] : m_Material->GetVariables()) {
			switch (type) {
			case UniformType::Bool: {
				SetValue(name, false);
				break;
			}
			case UniformType::Int: {
				SetValue(name, 0);
				break;
			}
			case UniformType::Float: {
				SetValue(name, 0.0f);
				break;
			}
			case UniformType::Float2: {
				SetValue(name, glm::vec2{0.0f, 0.0f});
				break;
			}
			case UniformType::Float3: {
				SetValue(name, glm::vec3{0.0f, 0.0f, 0.0f});
				break;
			}
			case UniformType::Float4: {
				SetValue(name, glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f });
				break;
			}
			case UniformType::Double: {
				SetValue(name, 0.0);
				break;
			}
			}
		}
	}

	MaterialTable::MaterialTable(uint32_t materialCount) :
		m_MaterialCount(materialCount) {}

	MaterialTable::MaterialTable(Ref<MaterialTable> other) :
		m_MaterialCount(other->m_MaterialCount) {
		const auto& materials = other->GetMaterials();
		for (auto [index, materialAsset] : materials)
			SetMaterial(index, materialAsset);
	}

	void MaterialTable::SetMaterial(uint32_t index, Ref<MaterialAsset> material) {
		m_Materials[index] = material;
		if (index >= m_MaterialCount)
			m_MaterialCount = index + 1;
	}

	void MaterialTable::ClearMaterial(uint32_t index) {
		SNOW_CORE_ASSERT(HasMaterial(index));
		m_Materials.erase(index);
		if (index >= m_MaterialCount)
			m_MaterialCount = index + 1;
	}

	void MaterialTable::Clear() {
		m_Materials.clear();
	}
}