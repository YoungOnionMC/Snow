#pragma once

#include <glm/vec3.hpp>
#include <Snow/Core/Ref.h>
#include <Snow/Math/Ray.h>

namespace Raytrace {
	class Material;
}

struct HitRecord {
	glm::vec3 pos;
	glm::vec3 normal;
	Snow::Ref<Raytrace::Material> mat;
	double t;
	bool front;

	void setFrontNormal(const Snow::Math::Ray& ray, const glm::vec3& outwardNormal) {
		front = glm::dot(ray.Direction, outwardNormal) < 0;
		normal = front ? outwardNormal : -outwardNormal;
	}
};