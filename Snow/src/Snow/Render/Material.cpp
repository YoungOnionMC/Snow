#include <spch.h>

#include "Snow/Render/Material.h"

namespace Snow {
	namespace Render {

		Material::Material(const Ref<Pipeline>& pipeline) :
			m_Pipeline(pipeline) {}

		void Material::Bind() {
			m_Pipeline->Bind();
		}

		MaterialInstance::MaterialInstance(const Ref<Material>& material) :
			m_Material(material) {
		
		}
	}
}