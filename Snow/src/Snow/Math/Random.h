#pragma once
#include "Snow/Core/Base.h"
#include <random>

#include <glm/glm.hpp>

namespace Snow {
	namespace Math {
		class Random {
		public:
			static uint32_t NextUInt(uint32_t min, uint32_t max) {
				return min + (s_UniformDistribution(s_Engine) % (max - min + 1));
			}

			static float NextFloat(float max = 1.0f) {
				return (float)s_UniformDistribution(s_Engine) / (float)std::numeric_limits<uint32_t>::max() * max;
			}

			static float NextFloat(float min, float max) {
				return (float)s_UniformDistribution(s_Engine) / (float)std::numeric_limits<uint32_t>::max() * (max - min) + min;
			}

			static glm::vec3 NextVec3() {
				return glm::vec3(NextFloat(), NextFloat(), NextFloat());
			}

			static glm::vec3 NextVec3(float min, float max) {
				return glm::vec3(NextFloat() * (max - min) + min, NextFloat() * (max - min) + min, NextFloat() * (max - min) + min);
			}

			static glm::vec4 NextVec4() {
				return glm::vec4(NextFloat(), NextFloat(), NextFloat(), NextFloat());
			}
		private:
			static std::random_device s_RandomDevice;
			static std::mt19937_64 s_Engine;
			static std::uniform_int_distribution<std::mt19937::result_type> s_UniformDistribution;
		};
	}
}