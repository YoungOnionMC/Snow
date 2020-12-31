#pragma once

#include "Snow/ImGui/ImGuiLayer.h"
#include "Snow/Platform/DirectX11/DirectXCommon.h"

namespace Snow {
	class DirectX11ImGuiLayer : public ImGuiLayer {
	public:
		DirectX11ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void BeginImGuiFrame() override;
		void EndImGuiFrame() override;
	};

}