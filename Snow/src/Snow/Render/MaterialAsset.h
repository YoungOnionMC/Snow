#pragma once


#include "Snow/Asset/Asset.h"

#include "Snow/Render/Material.h"
#include "Snow/Core/Log.h"

#include <map>

namespace Snow {
	class MaterialAsset : public Asset {
	public:
		MaterialAsset();
		MaterialAsset(Ref<Render::Material> material);
		~MaterialAsset();

		void SetTexture(const std::string& name, Ref<Render::Texture2D> texture);
		void ClearTexture(const std::string& name);
		Ref<Render::Texture2D> GetTexture(const std::string& name);

		bool IsUsingTexture(const std::string& name);

		void SetValue(const std::string& name, bool value);
		void SetValue(const std::string& name, float value);
		void SetValue(const std::string& name, double value);
		void SetValue(const std::string& name, int value);
		void SetValue(const std::string& name, const glm::vec2& value);
		void SetValue(const std::string& name, const glm::vec3& value);
		void SetValue(const std::string& name, const glm::vec4& value);

		void SetValuesInMap();

		Ref<Render::Material> GetMaterial() const { return m_Material; }

	private:
		Ref<Render::Material> m_Material;
	};

	class MaterialTable : public RefCounted {
	public:
		MaterialTable(uint32_t materialCount = 1);
		MaterialTable(Ref<MaterialTable> other);
		~MaterialTable() = default;

		bool HasMaterial(uint32_t materialIndex) const { return m_Materials.find(materialIndex) != m_Materials.end(); }
		void SetMaterial(uint32_t index, Ref<MaterialAsset> material);
		void ClearMaterial(uint32_t index);

		Ref<MaterialAsset> GetMaterial(uint32_t materialIndex) const {
			SNOW_CORE_ASSERT(HasMaterial(materialIndex));
			return m_Materials.at(materialIndex);
		}

		std::map<uint32_t, Ref<MaterialAsset>>& GetMaterials() { return m_Materials; }
		const std::map<uint32_t, Ref<MaterialAsset>>& GetMaterials() const { return m_Materials; }

		uint32_t GetMaterialCount() const { return m_MaterialCount; }
		void SetMaterialCount(uint32_t materialCount) { m_MaterialCount = materialCount; }

		void Clear();

	private:
		std::map<uint32_t, Ref<MaterialAsset>> m_Materials;
		uint32_t m_MaterialCount;
	};
}