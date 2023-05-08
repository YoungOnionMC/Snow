#pragma once

#include <Snow.h>
#include "Snow/Math/Ray.h"

#include "RayTracing/RayCamera.h"

using namespace Snow;

class RayTracingLayer : public Snow::Core::Layer {

	struct Sphere {
		glm::vec3 Pos;
		float radius;
		glm::vec3 Color;
	};

public:
	RayTracingLayer() :
		m_Camera(45.0f, 0.1f, 1000.0f) {
	
			{
				Sphere s;
				s.Pos = glm::vec3(-3, 0, 0);
				s.Color = glm::vec3(1, 0, .6);
				s.radius = .4f;
				m_Spheres.push_back(s);
			}
			{
				Sphere s;
				s.Pos = glm::vec3(1, .6, 0);
				s.Color = glm::vec3(.5, .87, .6);
				s.radius = .5f;
				m_Spheres.push_back(s);
			}
	}

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Snow::Timestep ts) override;

	void OnImGuiRender();

	void OnResize(uint32_t width, uint32_t height);

	void OnRender(RayCamera& camera);


private:

	glm::vec4 TraceRay(std::vector<Sphere>& spheres, Math::Ray ray);

	std::vector<Sphere> m_Spheres;

	Ref<Snow::Render::Texture2D> m_OutputTexture;
	Ref<Snow::Render::Image2D> m_OutputImage;

	RayCamera m_Camera;

	glm::vec2 m_ViewportSize{1920, 1080};

	std::vector<uint32_t> m_ImageData;


};