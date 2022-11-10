#pragma once

#include "Snow/Render/Material.h"

#include "Snow/Editor/Node/Node.h"

namespace Snow {
	class MaterialPanel {
	public:
		MaterialPanel(Ref<Render::Material>& material);

		void OnImGuiRender();
	private:
		Ref<Render::Material> m_Material;

		std::vector<Node::TextureNode> m_TextureNodes;



	};
}