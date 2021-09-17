#pragma once

#include <glad/glad.h>

#include "Snow/Core/Log.h"

namespace Snow {
	namespace Render {

#define GLCheckError(x) x; \
		GLLogCall(#x, __FILE__, __LINE__);

		static void GLLogCall(const char* func, const char* file, int line) {
			GLenum result = glGetError();
			if (result != GL_NO_ERROR) {
				SNOW_CORE_ERROR("[OpenGL Error {0}] ({1}): {2}|{3}", result, func, file, line);
				//SNOW_CORE_ASSERT(false);
			}
		}
	}
}