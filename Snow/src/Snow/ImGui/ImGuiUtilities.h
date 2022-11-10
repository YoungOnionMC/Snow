#pragma once

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <imgui_internal.h>


#include "Snow/Render/Texture.h"
#include "Snow/ImGui/Colors.h"
#include "Snow/Utils/StringUtils.h"

#include <vector>
#include <string>
#include <utility>

namespace Snow {
	namespace UI {
		ImTextureID GetTextureID(Ref<Render::Texture2D> texture);

		class ScopedStyle {
		public:
			ScopedStyle(const ScopedStyle&) = delete;
			ScopedStyle operator=(const ScopedStyle&) = delete;
			template<typename T>
			ScopedStyle(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }
			~ScopedStyle() { ImGui::PopStyleVar(); }
		};

		class ScopedStyleStack {
		public:
			ScopedStyleStack(const ScopedStyleStack&) = delete;
			ScopedStyleStack operator=(const ScopedStyleStack&) = delete;

			template<typename ValueType, typename... OtherStylePairs>
			ScopedStyleStack(ImGuiStyleVar firstStyleVar, ValueType firstValue, OtherStylePairs&&... otherStylePairs) :
				m_Count((sizeof... (otherStylePairs) / 2) + 1) {
				static_assert((sizeof... (otherStylePairs) & 1u) == 0, "ScopedStyleStack constructor expects a list of pairs of StyleVars and values");

				PushStyle(firstStyleVar, firstValue, std::forward<OtherStylePairs>(otherStylePairs)...);
			}

			~ScopedStyleStack() { ImGui::PopStyleVar(m_Count); }

		private:
			template<typename ValueType, typename... OtherStylePairs>
			void PushStyle(ImGuiStyleVar styleVar, ValueType value, OtherStylePairs&&... otherStylePairs) {
				if constexpr (sizeof... (otherStylePairs) == 0) {
					ImGui::PushStyleVar(styleVar, value);
				}
				else {
					ImGui::PushStyleVar(styleVar, value);
					PushStyle(std::forward<OtherStylePairs>(otherStylePairs)...);
				}
			}

			int m_Count;
		};

		class ScopedColor {
		public:
			ScopedColor(const ScopedColor&) = delete;
			ScopedColor operator=(const ScopedColor&) = delete;
			template<typename T>
			ScopedColor(ImGuiCol colorID, T color) { ImGui::PushStyleColor(colorID, color); }
			~ScopedColor() { ImGui::PopStyleColor(); }
		};

		class ScopedColorStack {
		public:
			ScopedColorStack(const ScopedColorStack&) = delete;
			ScopedColorStack operator=(const ScopedColorStack&) = delete;

			template<typename ColorType, typename... OtherColorPairs>
			ScopedColorStack(ImGuiCol firstColorID, ColorType firstValue, OtherColorPairs&&... otherColorPairs) :
				m_Count((sizeof... (otherColorPairs) / 2) + 1) {
				static_assert((sizeof... (otherColorPairs) & 1u) == 0, "ScopedColorStack constructor expects a list of pairs of ColorIDs and colors");

				PushColor(firstColorID, firstValue, std::forward<OtherColorPairs>(otherColorPairs)...);
			}

			~ScopedColorStack() { ImGui::PopStyleColor(m_Count); }

		private:
			template<typename ColorType, typename... OtherColor>
			void PushColor(ImGuiCol colorID, ColorType color, OtherColor&&... otherColorPairs) {
				if constexpr (sizeof... (otherColorPairs) == 0) {
					ImGui::PushStyleColor(colorID, color);
				}
				else {
					ImGui::PushStyleColor(colorID, color);
					ScopedColorStack::PushColor(std::forward<OtherColor>(otherColorPairs)...);
				}
			}

			int m_Count;
		};

		class ScopedFont {
		public:
			ScopedFont(const ScopedFont&) = delete;
			ScopedFont operator=(const ScopedFont&) = delete;
			ScopedFont(ImFont* font) { ImGui::PushFont(font); }
			~ScopedFont() { ImGui::PopFont(); }
		};

		// Colors

		static ImU32 ColorWithValue(const ImColor& color, float value) {
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, sat, std::min(value, 1.0f));
		}

		static ImU32 ColorWithSaturation(const ImColor& color, float saturation) {
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, std::min(saturation, 1.0f), val);
		}

		static ImU32 ColorWithHue(const ImColor& color, float hue) {
			const ImVec4& colRow = color.Value;
			float h, s, v;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, h, s, v);
			return ImColor::HSV(std::min(hue, 1.0f), s, v);
		}

		static ImU32 ColorWithMultipliedValue(const ImColor& color, float multiplier) {
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
		}

		static ImU32 ColorWithMultipliedSaturation(const ImColor& color, float multiplier) {
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, std::min(sat * multiplier, 1.0f), val);
		}

		static ImU32 ColorWithMultipliedHue(const ImColor& color, float multiplier) {
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(std::min(hue * multiplier, 1.0f), sat, val);
		}

		// Rectangles

		static inline ImRect GetItemRect() {
			return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
		}

		static inline ImRect RectExpanded(const ImRect& rect, float x, float y) {
			ImRect result = rect;
			result.Min.x -= x;
			result.Min.y -= y;
			result.Max.x += x;
			result.Max.y += y;
			return result;
		}

		static inline ImRect RectOffset(const ImRect& rect, float x, float y) {
			ImRect result = rect;
			result.Min.x += x;
			result.Min.y += y;
			result.Max.x += x;
			result.Max.y += y;
			return result;
		}

		static inline ImRect RectOffset(const ImRect& rect, ImVec2 xy) {
			return RectOffset(rect, xy.x, xy.y);
		}

		// Window 
		static bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0) {
			bool opened = false;

			if (ImGui::BeginPopup(str_id, flags)) {
				opened = true;

				const float padding = ImGui::GetStyle().WindowBorderSize;
				const ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), -padding, -padding);
				ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
				const ImColor col1 = Colors::Theme::background;
				const ImColor col2 = UI::ColorWithMultipliedValue(col1, 0.8f);
				ImGui::GetWindowDrawList()->AddRectFilledMultiColor(windowRect.Min, windowRect.Max, col1, col1, col2, col2);
				ImGui::PopClipRect();

				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 80));
			}

			return opened;
		}

		static void EndPopup() {
			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}


		static void DrawButtonImage(const Ref<Render::Texture2D>& imageN, const Ref<Render::Texture2D>& imageH, const Ref<Render::Texture2D>& imageP, ImU32 tintN, ImU32 tintH, ImU32 tintP, ImVec2 rectMin, ImVec2 rectMax) {
			auto* drawList = ImGui::GetWindowDrawList();
			if (ImGui::IsItemActive())
				drawList->AddImage(GetTextureID(imageP), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintP);
			else if (ImGui::IsItemHovered())
				drawList->AddImage(GetTextureID(imageH), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintH);
			else
				drawList->AddImage(GetTextureID(imageN), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintN);
		}

		static void DrawButtonImage(const Ref<Render::Texture2D>& imageN, const Ref<Render::Texture2D>& imageH, const Ref<Render::Texture2D>& imageP, ImU32 tintN, ImU32 tintH, ImU32 tintP, ImRect rect) {
			DrawButtonImage(imageN, imageH, imageP, tintN, tintH, tintP, rect.Min, rect.Max);
		}

		static void DrawButtonImage(const Ref<Render::Texture2D>& imageN, const Ref<Render::Texture2D>& imageH, const Ref<Render::Texture2D>& imageP, ImU32 tintN, ImU32 tintH, ImU32 tintP) {
			DrawButtonImage(imageN, imageH, imageP, tintN, tintH, tintP, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
		}

		static void DrawButtonImage(const Ref<Render::Texture2D>& image, ImU32 tintN, ImU32 tintH, ImU32 tintP, ImVec2 rectMin, ImVec2 rectMax) {
			DrawButtonImage(image, image, image, tintN, tintH, tintP, rectMin, rectMax);
		}

		static void DrawButtonImage(const Ref<Render::Texture2D>& image, ImU32 tintN, ImU32 tintH, ImU32 tintP, ImRect rect) {
			DrawButtonImage(image, image, image, tintN, tintH, tintP, rect.Min, rect.Max);
		}
	}
}