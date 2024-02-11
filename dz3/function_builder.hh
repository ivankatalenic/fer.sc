#pragma once

#include "int_unary_function.hh"

namespace FunctionBuilder {
	IntUnaryFunction const* lFunction(int max, int min);
	IntUnaryFunction const* gammaFunction(int min, int max);
	IntUnaryFunction const* lambdaFunction(int left, int mid, int right);
	IntUnaryFunction const* constantFunction();

	IntUnaryFunction const* combineZadehOr(
		IntUnaryFunction const* a,
		IntUnaryFunction const* b
	);
}
