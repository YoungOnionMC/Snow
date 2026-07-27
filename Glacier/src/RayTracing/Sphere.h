#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
public:
	Sphere(const glm::vec3& center, double radius, Snow::Ref<Material> mat) : m_Center(center), m_Radius(std::max(0.0, radius)), m_Mat(mat) {}

	bool hit(const Snow::Math::Ray& ray, Interval rayTrace, HitRecord& record) const override {
		glm::vec3 oc = m_Center - ray.Origin;
		double a = glm::dot(ray.Direction, ray.Direction);
		double h = glm::dot(ray.Direction, oc);
		double c = glm::dot(oc, oc) - m_Radius * m_Radius;

		double discriminant = h * h - a * c;
		if (discriminant < 0) return false;

		double sqrtDiscriminant = std::sqrt(discriminant);
		double root = (h - sqrtDiscriminant) / a;
		if (!rayTrace.surrounds(root)) {
			root = (h + sqrtDiscriminant) / a;
			if (!rayTrace.surrounds(root)) {
				return false;
			}
		}

		record.t = root;
		record.pos = ray.Origin + ray.Direction * glm::vec3(record.t);
		glm::vec3 normal = (record.pos - m_Center) / glm::vec3(m_Radius);
		record.setFrontNormal(ray, normal);
		record.mat = m_Mat;
		return true;
	}

private:
	glm::vec3 m_Center;
	double m_Radius;
	Snow::Ref<Raytrace::Material> m_Mat;
};