#pragma once

#include <Snow/Math/Ray.h>
#include "Interval.h"
#include "HitRecord.h"
#include <Snow/Core/Ref.h>

class Hittable : public Snow::RefCounted {
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Snow::Math::Ray& ray, Interval rayTrace, HitRecord& record) const = 0;
};