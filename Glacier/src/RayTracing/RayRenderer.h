#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "RayCamera.h"
#include "Snow/Math/Ray.h"

//#include <Snow.h>

using namespace Snow;
class RayRenderer {
public:
	struct Sphere {
		glm::vec3 Pos;
		float radius;
		glm::vec3 Color;
	};

	struct Scene {
		std::vector<Sphere> Spheres;
	};

	struct HitInfo {
		bool Hit;
		float HitDistance;

		glm::vec3 WorldPos;
		glm::vec3 WorldNormal;

		uint32_t ObjectIndex;
	};

public:

	void OnResize(uint32_t width, uint32_t height);

	void OnRender(const Scene& scene, const RayCamera& camera);

	const Ref<Snow::Render::Image2D>& GetOutputImage() const { return m_OutputImage; }

	int& GetBounces() { return m_Bounces; }

private:

	glm::vec4 RayGen(uint32_t x, uint32_t y);

	HitInfo TraceRay(const Math::Ray& ray);
	HitInfo ClosestHit(const Math::Ray& ray, float hitDistance, uint32_t objectIndex);
	HitInfo Miss(const Math::Ray& ray);

	const Scene* m_ActiveScene;
	const RayCamera* m_ActiveCamera;

	int m_Bounces = 1;

	glm::vec2 m_ViewportSize;

	Ref<Snow::Render::Image2D> m_OutputImage;
	std::vector<uint32_t> m_ImageData;
};