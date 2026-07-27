#pragma once

#include <limits>

struct Interval {
public:
	double min, max;

	Interval() : min(std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}

	Interval(double min, double max) : min(min), max(max) {}

	inline double size() const {
		return max - min;
	}

	inline bool contains(double x) const {
		return min <= x && x <= max;
	}

	inline bool surrounds(double x) const {
		return min < x && x < max;
	}

	inline double clamp(double x) const {
		return x < min ? min : x > max ? max : x;
	}

	static const Interval s_Empty, s_Universe;
};