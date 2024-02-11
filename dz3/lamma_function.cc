#include "lamma_function.hh"

#include <stdexcept>

LammaFunction::LammaFunction(int l, int r, bool rising): left{l}, right{r}, isRising{rising} {
	if (left > right) {
		throw std::invalid_argument("the left point must be greater than the right point");
	}
}

double LammaFunction::valueAt(int val) const {
	const int diff{right - left};
	const double step{1.0 / diff};

	if (isRising) {
		if (val >= right) {
			return 1.0;
		}
		if (val <= left) {
			return 0.0;
		}

		return (val - left) * step;
	} else {
		if (val <= left) {
			return 1.0;
		}
		if (val >= right) {
			return 0.0;
		}

		return 1.0 - (val - left) * step;
	}
}
