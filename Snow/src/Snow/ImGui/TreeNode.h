#pragma once

#include "Snow/ImGui/ImGui.h"

#define IMGUI_ENABLE_TEST_ENGINE
#include <imgui.h>
#include <imgui_internal.h>

namespace Snow {
	namespace UI {
		bool TreeNodeWithIcon(Ref<Render::Texture2D> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end, ImColor iconTint = IM_COL32_WHITE);
	}
}