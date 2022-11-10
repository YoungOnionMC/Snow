#pragma once

#include <imgui.h>

namespace Colors {

	static inline float ConvertSRGBFromLinear(float linearValue);
	static inline float ConvertSRGBToLinear(float srgbValue);
	ImVec4 ConvertFromSRGB(ImVec4 color);
	ImVec4 ConvertToSRGB(ImVec4 color);

	namespace Theme {
		constexpr auto background = IM_COL32(30, 30, 30, 255);
		constexpr auto backgroundBright = IM_COL32(30, 30, 30, 255);
		constexpr auto backgroundDark = IM_COL32(20, 20, 20, 255);
		constexpr auto titlebar = IM_COL32(141, 165, 140, 255); // Morning Blue

		constexpr auto windowBG = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto childBG = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto popupBG = IM_COL32(141, 165, 140, 255); // Morning Blue

		constexpr auto border = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto borderShadow = IM_COL32(141, 165, 140, 255); // Morning Blue

		constexpr auto frameBG = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto frameBGHovered = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto frameBGActive = IM_COL32(141, 165, 140, 255); // Morning Blue

		constexpr auto titleBG = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto titleBGActive = IM_COL32(141, 165, 140, 255); // Morning Blue
		constexpr auto titleBGCollapsed = IM_COL32(141, 165, 140, 255); // Morning Blue

		constexpr auto text = IM_COL32(186, 186, 186, 255);
		constexpr auto textBright = IM_COL32(222, 222, 222, 255);
		constexpr auto textDark = IM_COL32(152, 152, 152, 255);

		constexpr auto highlight = IM_COL32(30, 30, 30, 255);
		constexpr auto muted = IM_COL32(204, 200, 155, 255); // Dark Vanilla
		constexpr auto selection = IM_COL32(60, 64, 70, 255); // Arsenic
		constexpr auto selectionMuted = IM_COL32(52, 56, 64, 255); // Arsenic Muted

	}
}
