#include "combine_zadeh_or_function.hh"

#include <stdexcept>

CombineZadehOrFunction::CombineZadehOrFunction(
	IntUnaryFunction const* aa,
	IntUnaryFunction const* bb
): a(aa), b(bb) {
	if (a == nullptr) {
		throw std::invalid_argument("the first unary function is nul");
	}
	if (b == nullptr) {
		throw std::invalid_argument("the second unary function is null");
	}
}

static double max(double a, double b) {
	if (a >= b) {
		return a;
	}
	return b;
}

double CombineZadehOrFunction::valueAt(int x) const {
	return max(a->valueAt(x), b->valueAt(x));
}
