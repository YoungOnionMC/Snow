#pragma once

#include <Snow.h>

using namespace Snow;

class RayTracingLayer : public Snow::Core::Layer {

public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Snow::Timestep ts) override;

	void OnImGuiRender();

	void OnResize(uint32_t width, uint32_t height);


private:

	Ref<Snow::Render::Texture2D> m_OutputTexture;
	Ref<Snow::Render::Image2D> m_OutputImage;

	glm::vec2 m_ViewportSize{1920, 1080};

	uint32_t* m_ImageData;


};