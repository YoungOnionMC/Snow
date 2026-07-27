#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "RayCamera.h"
#include "HittableList.h"
#include <Snow/Math/Ray.h>

//#include <Snow.h>

using namespace Snow;
class RayRenderer {

public:

	void OnResize(uint32_t width, uint32_t height);

	void OnRender(const RayCamera& camera);

	const Ref<Snow::Render::Image2D>& GetOutputImage() const { return m_OutputImage; }

	int& GetBounces() { return m_Bounces; }

	inline void SetWorld(HittableList world) { m_World = &world; }
	HittableList& GetWorld() { return *m_World; }

private:

	glm::vec3 rayColor(const Snow::Math::Ray& ray, int depth, const Hittable& world);

	//glm::vec4 RayGen(uint32_t x, uint32_t y);

	/*HitInfo TraceRay(const Math::Ray& ray);
	HitInfo ClosestHit(const Math::Ray& ray, float hitDistance, uint32_t objectIndex);
	HitInfo Miss(const Math::Ray& ray);*/

	HittableList* m_World = new HittableList();
	const RayCamera* m_ActiveCamera;

	int m_Bounces = 5;

	glm::vec2 m_ViewportSize;

	Ref<Snow::Render::Image2D> m_OutputImage;
	std::vector<uint32_t> m_ImageData;
};