#pragma once

#include <Snow.h>


#include "RayTracing/RayCamera.h"
#include "RayRenderer.h"

using namespace Snow;

class RayTracingLayer : public Snow::Core::Layer {

	

public:
	RayTracingLayer() :
		m_Camera(45.0f, 0.1f, 1000.0f) {
			{
				RayRenderer::Sphere s;
				s.Pos = glm::vec3(0, -270, 0);
				s.Color = glm::vec3(.2, .3, .6);
				s.radius = 250.f;
				m_Scene.Spheres.push_back(s);
			}
			for (int i = 0; i < 30; i++) {
				RayRenderer::Sphere s;
				s.Pos = glm::vec3(Math::Random::NextFloat(-10, 10), 0.0f, Math::Random::NextFloat(-10, 10));
				s.Color = Math::Random::NextVec3();
				s.radius = Math::Random::NextFloat();
				m_Scene.Spheres.push_back(s);
			}
	}

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Snow::Timestep ts) override;
	void OnEvent(Snow::Core::Event::Event& e) override;

	void OnImGuiRender();

	void OnResize(uint32_t width, uint32_t height);


private:

	
	RayRenderer m_Renderer;
	RayRenderer::Scene m_Scene;
	RayCamera m_Camera;

	bool m_FirstFrame = false;

	Ref<Snow::Render::Texture2D> m_OutputTexture;
	


	glm::vec2 m_ViewportSize{1920, 1080};

};