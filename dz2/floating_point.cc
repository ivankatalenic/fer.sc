#include "floating_point.hh"

#include <limits>
#include <cmath>

/**
 * This code is copied from:
 * https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
 */
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}

bool FloatingPoint::isEqual(const float a, const float b) {
	return almost_equal(a, b, FloatingPoint::PRECISION_FLOAT);
}

bool FloatingPoint::isEqual(const double a, const double b) {
	return almost_equal(a, b, FloatingPoint::PRECISION_DOUBLE);
}
