#pragma once

#include <imgui.h>

namespace Colors {

	static inline float ConvertSRGBFromLinear(float linearValue);
	static inline float ConvertSRGBToLinear(float srgbValue);
	ImVec4 ConvertFromSRGB(ImVec4 color);
	ImVec4 ConvertToSRGB(ImVec4 color);

	namespace Theme {
		constexpr auto background = IM_COL32(30, 30, 30, 255);
		constexpr auto titlebar = IM_COL32(30, 30, 30, 255);
		constexpr auto text = IM_COL32(30, 30, 30, 255);
		constexpr auto textBright = IM_COL32(30, 30, 30, 255);
		constexpr auto textDark = IM_COL32(30, 30, 30, 255);
		constexpr auto highlight = IM_COL32(30, 30, 30, 255);
		constexpr auto selection = IM_COL32(30, 30, 30, 255);

	}
}
