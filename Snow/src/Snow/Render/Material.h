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
			void Push(const T& value);

			void Push(const Ref<API::Texture2D>& texture);

			Ref<Pipeline> GetPipeline() { return m_Pipeline; }


		private:
			Ref<Pipeline> m_Pipeline;

			std::unordered_set<MaterialInstance*> m_MaterialInstances;

			Buffer m_UniformStorageBuffer;
			std::vector<Ref<API::Texture>> m_Textures;
		};

		class MaterialInstance : public RefCounted {
		public:
			MaterialInstance(const Ref<Material>& material);

			Ref<Material> GetMaterial() { return m_Material; }

		private:
			Ref<Material> m_Material;
		};
	}
}
