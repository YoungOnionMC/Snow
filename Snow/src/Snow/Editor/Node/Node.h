#pragma once

#include <stdint.h>
#include <string>

#include "Snow/Core/Buffer.h"

#include "Snow/Render/Texture.h"

namespace Snow {
	namespace Node {

		struct Node {
			uint32_t Width;
			uint32_t Height;

			std::string Name;
		};

		struct Variable {
			std::string Name;
			std::string Type;

			Buffer Data;
		};

		struct VariableNode : public Node {
			std::vector<Variable> Variables;
		};

		struct TextureNode : public Node {
			std::vector<Variable> Variables;

			Ref<Render::Texture2D> Texture;
		};
	}
}