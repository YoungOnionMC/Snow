#pragma once

#include <Snow/Math/Ray.h>
#include <Snow/Core/Ref.h>
#include "HitRecord.h"
#include "Util.h"

namespace Raytrace {
	class Material : public Snow::RefCounted{
	public:
		virtual ~Material() = default;

		virtual bool scatter(const Snow::Math::Ray& ray, const HitRecord& record, glm::vec3& attenuation, Snow::Math::Ray& scattered) const {
			return false;
		}
	};

	class Lambertian : public Raytrace::Material {
	public:
		Lambertian(const glm::vec3& albedo) : m_Albedo(albedo) {}

		bool scatter(const Snow::Math::Ray& ray, const HitRecord& record, glm::vec3& attenuation, Snow::Math::Ray& scattered) const override {
			glm::vec3 scatterDir = record.normal + randomUnitVector();
			if (glm::dot(scatterDir, scatterDir) < std::numeric_limits<float>::epsilon()) {
				scatterDir = record.normal;
			}
			scattered = Snow::Math::Ray(record.pos, scatterDir);
			attenuation = m_Albedo;
			return true;
		}

	private:
		glm::vec3 m_Albedo;
	};

	class Metal : public Raytrace::Material {
	public:
		Metal(const glm::vec3& albedo, float fuzz) : m_Albedo(albedo), m_Fuzz(fuzz) {}

		bool scatter(const Snow::Math::Ray& ray, const HitRecord& record, glm::vec3& attenuation, Snow::Math::Ray& scattered) const override {
			glm::vec3 reflect = glm::reflect(ray.Direction, record.normal);
			reflect = glm::normalize(reflect) + m_Fuzz * randomUnitVector();
			scattered = Snow::Math::Ray(record.pos, reflect);
			attenuation = m_Albedo;
			return glm::dot(scattered.Direction, record.normal) > 0;
		}

	private:
		glm::vec3 m_Albedo;
		float m_Fuzz;
	};

	class Dielectric : public Raytrace::Material {
	public:
		Dielectric(float refraction) : m_RefractionIndex(refraction) {}

		bool scatter(const Snow::Math::Ray& ray, const HitRecord& record, glm::vec3& attenuation, Snow::Math::Ray& scattered) const override {
			attenuation = glm::vec3(1.0);
			float ri = record.front ? (1.0 / m_RefractionIndex) : m_RefractionIndex;

			glm::vec3 unitDir = glm::normalize(ray.Direction);

			double cosTheta = std::min(glm::dot(-unitDir, record.normal), 1.0f);
			double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
			bool cannotRefract = ri * sinTheta > 1.0;
			glm::vec3 dir = cannotRefract ? glm::reflect(unitDir, record.normal) : glm::refract(unitDir, record.normal, ri);
			scattered = Snow::Math::Ray(record.pos, dir);
			return true;
		}

	private:
		float m_RefractionIndex;
	};
}