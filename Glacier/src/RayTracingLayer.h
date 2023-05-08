#pragma once

#include <Snow.h>
#include "Snow/Math/Ray.h"

#include "RayTracing/RayCamera.h"

using namespace Snow;

class RayTracingLayer : public Snow::Core::Layer {

public:
	RayTracingLayer() :
		m_Camera(45.0f, 0.1f, 1000.0f) {}

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Snow::Timestep ts) override;

	void OnImGuiRender();

	void OnResize(uint32_t width, uint32_t height);

	void OnRender(RayCamera& camera);


private:

	glm::vec4 TraceRay(Math::Ray ray);


	Ref<Snow::Render::Texture2D> m_OutputTexture;
	Ref<Snow::Render::Image2D> m_OutputImage;

	RayCamera m_Camera;

	glm::vec2 m_ViewportSize{1920, 1080};

	std::vector<uint32_t> m_ImageData;


};