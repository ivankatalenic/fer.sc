#include "lambda_function.hh"

#include <stdexcept>

LambdaFunction::LambdaFunction(int l, int m, int r):
	left{l}, mid{m}, right{r}, rise(left, mid, true), fall(mid, right, false) {
	if (left > mid) {
		throw std::invalid_argument("the left point must be smaller than or equal to the mid point");
	}
	if (right < mid) {
		throw std::invalid_argument("the right point must be bigger than or equal to the mid point");
	}
}

double LambdaFunction::valueAt(int e) const {
	if (left == right) {
		return 0.0;
	}

	return rise.valueAt(e) * fall.valueAt(e);
}