#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Snow {
	namespace UI {
		extern void BeginGrid(uint32_t columns);
		extern void EndGrid();
		extern bool DrawStringControl(const char* label, std::string& value, bool error = false);
		extern bool DrawStringControl(const char* label, const char* value);
		extern bool DrawBoolControl(const char* label, bool* value);
		extern bool DrawCharControl(const char* label, char* value);
		extern bool DrawIntControl(const char* label, int* value);
		extern bool DrawFloatControl(const char* label, float* value);
		extern bool DrawFloat2Control(const char* label, glm::vec2& values, float delta = 0.1f);
		extern bool DrawFloat3Control(const char* label, glm::vec3& values, float delta = 0.1f);
		extern bool DrawFloat4Control(const char* label, glm::vec4& values, float delta = 0.1f);
		extern bool DrawColorControl(const char* label, glm::vec4& values);

		extern void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	}
}