#pragma once

#include <stdint.h>

#include <string>

namespace Snow {

	namespace Shader {

		enum class Type : int16_t {
			None = -1,
			Void = 0,
			Bool = 1,
			Char = 2,
			UnsignedChar = 3,
			Int = 4,
			UnsignedInt = 5,
			Long = 6,
			UnsignedLong = 7,
			Float = 8,
			Double = 9,
			Mat2 = 10,
			Mat3 = 11,
			Mat4 = 12,

			Uniform = 13,
			Struct = 14,
		};

		size_t GetTypeSize(Type type) {
			switch (type) {
			case Type::Bool:	return 1;
			case Type::Char:	return 1;
			case Type::UnsignedChar:	return 1;
			case Type::Int:	return 2;
			case Type::UnsignedInt:	return 2;
			case Type::Long:	return 4;
			case Type::UnsignedLong:	return 4;
			case Type::Float:	return 2;
			case Type::Double:	return 4;
			case Type::Mat2:	return 2 * 2 * 2;
			case Type::Mat3:	return 2 * 3 * 3;
			case Type::Mat4:	return 2 * 4 * 4;
			}
		}



		struct Variable {
			std::string Name;
			Type _Type;

			void* data;
		};

		struct Function {
			std::string Name;
			Type* ReturnTypes;
			Type* ParamTypes;
		};

	}




}