#pragma once

namespace FloatingPoint {
	constexpr int PRECISION_FLOAT{5};
	constexpr int PRECISION_DOUBLE{10};

	bool isEqual(const float a, const float b);
	bool isEqual(const double a, const double b);
};
