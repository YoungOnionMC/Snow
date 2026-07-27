#pragma once

#include "Hittable.h"
#include <vector>
#include <Snow/Core/Ref.h>

class HittableList : public Hittable {
public:
	std::vector<Snow::Ref<Hittable>> m_Objects;

	HittableList() {}
	HittableList(Snow::Ref<Hittable> object) { add(object); }

	inline void clear() {
		m_Objects.clear();
	}

	inline void add(Snow::Ref<Hittable> object) {
		m_Objects.push_back(object);
	}

	bool hit(const Snow::Math::Ray& ray, Interval rayTrace, HitRecord& record) const override {
		HitRecord temp;
		bool hitAnything = false;
		double closest = rayTrace.max;

		for (const auto& object : m_Objects) {
			if (object->hit(ray, Interval(rayTrace.min, closest), temp)) {
				hitAnything = true;
				closest = temp.t;
				record = temp;
			}
		}
		return hitAnything;
	}
};