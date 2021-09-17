#include <spch.h>

#include "Snow/Render/ShaderUniform.h"

namespace Snow {
	namespace Render {
		ShaderUniform::ShaderUniform(const std::string& name, ShaderStruct* sStruct, uint32_t count) :
			Name(name), Type(UniformType::Struct), Struct(sStruct), Count(count), Size(sStruct->GetSize()* count), Offset(0) {}

		uint32_t ShaderUniform::GetAbsoluteOffset() const { 
			return Struct ? Struct->GetOffset() : Offset; 
		}

		void ShaderUniform::SetOffset(uint32_t offset) {
			Struct ? Struct->SetOffset(offset) : Offset = offset;
		}
	}
}