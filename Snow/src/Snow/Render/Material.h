#pragma once

#include "Snow/Render/Shader/Shader.h"

#include "Snow/Core/Ref.h"

#include "Snow/Render/Pipeline.h"

#include "Snow/Render/API/Texture.h"

#include <unordered_set>

namespace Snow {
	namespace Render {
		class Material : public RefCounted {
			friend class MaterialInstance;
		public:
			Material(const Ref<Pipeline>& pipeline);

			void Bind();

			template<typename T>
			void Set(const std::string name, const T& value) {
				auto u = FindUniformDecl(name);
				auto& buffer = m_UniformStorageBuffer;
				buffer.Write((byte*)&value, u->GetSize(), u->GetOffset());

				for (auto mi : m_MaterialInstances)
					mi->OnMaterialValueUpdated(*u);
			}

			void Set(const std::string& name, const Ref<API::Texture>& texture) {
				auto tex = FindResourceDecl(name);
				uint32_t slot = tex->GetRegister();
				if (m_Textures.size() <= slot)
					m_Textures.resize((size_t)slot + 1);
				m_Textures[slot] = texture;
			}

			void Set(const std::string& name, const Ref<API::Texture2D>& texture) {
				Set(name, (const Ref<API::Texture>&)texture);
			}

			template<typename T>
			T& Get(const std::string& name) {
				auto u = FindUniformDecl(name);
				auto& buffer = m_UniformStorageBuffer;
				return buffer.Read<T>(u->GetOffset());
			}

			template<typename T>
			Ref<T> GetResource(const std::string& name) {
				auto tex = FindResourceDecl(name);
				uint32_t slot = tex->GetRegister();
				return m_Textures[slot];
			}

			Ref<Pipeline> GetPipeline() { return m_Pipeline; }

			static Ref<Material> Create(const Ref<Pipeline>& pipeline);
		private:
			void AllocateStorage();
			void BindTextures();

			const ShaderUniform* FindUniformDecl(const std::string& name);
			const ShaderResource* FindResourceDecl(const std::string& name);

			Ref<Pipeline> m_Pipeline;

			std::unordered_set<MaterialInstance*> m_MaterialInstances;

			Buffer m_UniformStorageBuffer;
			std::vector<Ref<API::Texture>> m_Textures;
		};

		class MaterialInstance : public RefCounted {
			friend class Material;
		public:
			MaterialInstance(const Ref<Material>& material, const std::string& name = "");
			virtual ~MaterialInstance();

			void Bind();

			template<typename T>
			void Set(const std::string& name, const T& value) {
				auto u = m_Material->FindUniformDecl(name);
				if (!u)
					return;

				auto& buffer = m_UniformStorageBuffer;
				buffer.Write((byte*)&value, u->GetSize(), u->GetOffset());

				m_OverriddenValues.insert(name);
			}

			void Set(const std::string& name, const Ref<API::Texture>& texture) {
				auto tex = m_Material->FindResourceDecl(name);
				if (!tex) {
					SNOW_CORE_WARN("Could not find material property: {0}", name);
					return;
				}
				uint32_t slot = tex->GetRegister();
				if (m_Textures.size() <= slot)
					m_Textures.resize((size_t)slot + 1);
				m_Textures[slot] = texture;
			}

			void Set(const std::string& name, const Ref<API::Texture2D>& texture) {
				Set(name, (const Ref<API::Texture>&)texture);
			}

			void Set(const std::string& name, const Ref<API::TextureCube>& texture) {
				Set(name, (const Ref<API::Texture>&)texture);
			}

			template<typename T>
			T& Get(const std::string& name) {
				auto u = m_Material->FindUniformDecl(name);

				auto& buffer = m_UniformStorageBuffer;
				return buffer.Read<T>(u->GetOffset());
			}

			template<typename T>
			Ref<T> GetResource(const std::string& name) {
				auto tex = m_Material->FindResourceDecl(name);

				uint32_t slot = tex->GetRegister();
				return Ref<T>(m_Textures[slot]);
			}

			static Ref<MaterialInstance> Create(const Ref<Material>& material);

			Ref<Material> GetMaterial() { return m_Material; }

		private:
			void AllocateStorage();
			void OnMaterialValueUpdated(const ShaderUniform& uniform);

			Ref<Material> m_Material;
			std::string m_Name;

			Buffer m_UniformStorageBuffer;
			std::vector<Ref<API::Texture>> m_Textures;

			std::unordered_set<std::string> m_OverriddenValues;
		};
	}
}
