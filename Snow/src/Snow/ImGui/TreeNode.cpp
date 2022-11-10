#include <spch.h>

#include "Snow/ImGui/TreeNode.h"

namespace Snow {
	namespace UI {
		bool TreeNodeWithIcon(Ref<Render::Texture2D> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end, ImColor iconTint) {
			using namespace ImGui;
			ImGuiWindow* window = GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			ImGuiLastItemData& lastItem = g.LastItemData;
			const ImGuiStyle& style = g.Style;
			const bool displayFrame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
			const ImVec2 padding = (displayFrame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

			if (!label_end)
				label_end = FindRenderedTextEnd(label);
			const ImVec2 labelSize = CalcTextSize(label, label_end, false);

			const float frameHeight = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), labelSize.y + padding.y * 2);
			ImRect frameBB;
			frameBB.Min.x = (flags & ImGuiTreeNodeFlags_SpanAvailWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
			frameBB.Min.y = window->DC.CursorPos.y;
			frameBB.Max.x = window->WorkRect.Max.x;
			frameBB.Max.y = window->DC.CursorPos.y + frameHeight;
			if (displayFrame) {
				frameBB.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
				frameBB.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
			}

			const float textOffsetX = g.FontSize + (displayFrame ? padding.x * 3 : padding.x * 2);
			const float textOffsetY = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);
			const float textWidth = g.FontSize + (labelSize.x > 0.0f ? labelSize.x + padding.x : 0.0f);
			ImVec2 textPos(window->DC.CursorPos.x + textOffsetX, window->DC.CursorPos.y + textOffsetY);
			ItemSize(ImVec2(textWidth, frameHeight), padding.y);

			ImRect interactBB = frameBB;
			if (!displayFrame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
				interactBB.Max.x = frameBB.Min.x + textWidth + style.ItemSpacing.x * 2.0f;

			const bool isLeaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
			bool isOpen = TreeNodeBehaviorIsOpen(id, flags);
			if (isOpen && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
				window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);

			bool itemAdd = ItemAdd(interactBB, id);
			lastItem.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
			lastItem.DisplayRect = frameBB;

			if (!itemAdd) {
				if (isOpen && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
					TreePushOverrideID(id);
				IMGUI_TEST_ENGINE_ITEM_INFO(lastItem.ID, label, lastItem.StatusFlags | (isLeaf ? 0 : ImGuiItemStatusFlags_Openable) | (isOpen ? ImGuiItemStatusFlags_Opened : 0));
				return isOpen;
			}

			ImGuiButtonFlags buttonFlags = ImGuiTreeNodeFlags_None;
			if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
				buttonFlags |= ImGuiButtonFlags_AllowItemOverlap;
			if (isLeaf)
				buttonFlags |= ImGuiButtonFlags_PressedOnDragDropHold;

			const float arrowHitx1 = (textPos.x - textOffsetX) - style.TouchExtraPadding.x;
			const float arrowHitx2 = (textPos.x - textOffsetX) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
			const bool isMouseXOverArrow = (g.IO.MousePos.x >= arrowHitx1 && g.IO.MousePos.x < arrowHitx2);

			if (window != g.HoveredWindow || !isMouseXOverArrow)
				buttonFlags |= ImGuiButtonFlags_NoKeyModifiers;

			if (isMouseXOverArrow)
				buttonFlags |= ImGuiButtonFlags_PressedOnClick;
			else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
				buttonFlags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
			else
				buttonFlags |= ImGuiButtonFlags_PressedOnClickRelease;

			bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
			const bool wasSelected = selected;

			bool hovered, held;
			bool pressed = ButtonBehavior(interactBB, id, &hovered, &held, buttonFlags);
			bool toggled = false;
			if (!isLeaf) {
				if (pressed && g.DragDropHoldJustPressedId != id) {
					if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
						toggled = true;
					if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
						toggled |= isMouseXOverArrow && !g.NavDisableMouseHover;
					if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
						toggled = true;
				}
				else if (pressed && g.DragDropHoldJustPressedId == id) {
					IM_ASSERT(buttonFlags & ImGuiButtonFlags_PressedOnDragDropHold);
					if (!isOpen)
						toggled = true;
				}

				if (g.NavId == id && g.NavMoveSubmitted && g.NavMoveDir == ImGuiDir_Left && isOpen) {
					toggled = true;
					NavMoveRequestCancel();
				}
				if (g.NavId == id && g.NavMoveSubmitted && g.NavMoveDir == ImGuiDir_Right && isOpen) {
					toggled = true;
					NavMoveRequestCancel();
				}

				if (toggled) {
					isOpen = !isOpen;
					window->DC.StateStorage->SetInt(id, isOpen);
					lastItem.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
				}
			}

			if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
				SetItemAllowOverlap();

			if (selected != wasSelected)
				lastItem.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

			const ImU32 arrowColor = selected ? IM_COL32(60, 60, 60, 255) : IM_COL32(120, 120, 120, 255);

			ImGuiNavHighlightFlags navHighlightFlags = ImGuiNavHighlightFlags_TypeThin;
			if (displayFrame) {
				const ImU32 bgColor = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : (hovered && !selected && !held && !pressed && !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);

				RenderFrame(frameBB.Min, frameBB.Max, bgColor, true, style.FrameRounding);
				RenderNavHighlight(frameBB, id, navHighlightFlags);

				if (flags & ImGuiTreeNodeFlags_Bullet)
					RenderBullet(window->DrawList, ImVec2(textPos.x - textOffsetX * 0.60f, textPos.y + g.FontSize * 0.5f), arrowColor);
				else if (!isLeaf)
					RenderArrow(window->DrawList, ImVec2(textPos.x - textOffsetX + padding.x, textPos.y), arrowColor, isOpen ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
				else
					textPos.x -= textOffsetX;

				if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
					frameBB.Max.x -= g.FontSize + style.FramePadding.x;

				if (icon) {
					auto itemID = lastItem.ID;
					auto itemFlags = lastItem.InFlags;
					auto itemStatusFlags = lastItem.StatusFlags;
					auto itemRect = lastItem.Rect;

					const float pad = 3.0f;
					const float arrowWidth = 20.0f + 1.0f;
					auto cursorPos = ImGui::GetCursorPos();
					//UI::ShiftCursor()
					UI::Image(icon, { frameHeight - pad * 2.0f, frameHeight - pad * 2.0f }, ImVec2(0, 0), ImVec2(1, 1), iconTint);

					ImGui::SetLastItemData(itemID, itemFlags, itemStatusFlags, itemRect);

					textPos.x += frameHeight + 2.0f;
				}

				textPos.y -= 1.0f;

				if (g.LogEnabled) {
					const char logPrefix[] = "\n##";
					const char logSuffix[] = "##";
					LogRenderedText(&textPos, logPrefix, logPrefix + 3);
					RenderTextClipped(textPos, frameBB.Max, label, label_end, &labelSize);
					LogRenderedText(&textPos, logSuffix, logSuffix + 2);
				}
				else {
					RenderTextClipped(textPos, frameBB.Max, label, label_end, &labelSize);
				}
			}
			else {
				if (hovered || selected) {
					const ImU32 bgColor = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : (hovered && !selected && !held && !pressed && !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);

					RenderFrame(frameBB.Min, frameBB.Max, bgColor, false);
					RenderNavHighlight(frameBB, id, navHighlightFlags);
				}

				if (flags & ImGuiTreeNodeFlags_Bullet)
					RenderBullet(window->DrawList, ImVec2(textPos.x - textOffsetX * 0.5f, textPos.y + g.FontSize * 0.5f), arrowColor);
				else if (!isLeaf)
					RenderArrow(window->DrawList, ImVec2(textPos.x - textOffsetX + padding.x, textPos.y + g.FontSize * 0.15f), arrowColor, isOpen ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);

				if (icon) {
					auto itemID = lastItem.ID;
					auto itemFlags = lastItem.InFlags;
					auto itemStatusFlags = lastItem.StatusFlags;
					auto itemRect = lastItem.Rect;

					const float pad = 3.0f;
					const float arrowWidth = 20.0f + 1.0f;
					auto cursorPos = ImGui::GetCursorPos();
					//UI::ShiftCursor()
					UI::Image(icon, { frameHeight - pad * 2.0f, frameHeight - pad * 2.0f }, ImVec2(0, 0), ImVec2(1, 1), iconTint);

					ImGui::SetLastItemData(itemID, itemFlags, itemStatusFlags, itemRect);

					textPos.x += frameHeight + 2.0f;
				}

				textPos.y -= 1.0f;

				if(g.LogEnabled)
					LogRenderedText(&textPos, ">");
				RenderText(textPos, label, label_end, false);
			}

			if (isOpen && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
				TreePushOverrideID(id);

			return isOpen;

			//return false;
		}
	}
}